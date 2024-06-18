#include "newfiledialog.h"
#include "ui_newfiledialog.h"
#include "QInputDialog"
#include "QTimer"

NewFileDialog::NewFileDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewFileDialog)
{
    ui->setupUi(this);
    connectSignalANDSlot();
    buttonGroup = new QButtonGroup(this);
    ui->radioButton_2->setChecked(true);
    buttonGroup->addButton(ui->radioButton);
    buttonGroup->addButton(ui->radioButton_2);

}

/**
 * @brief NewFileDialog::connectSignalANDSlot
 * 信号和槽连接的函数
 */
void NewFileDialog::connectSignalANDSlot()
{
    /**
     * @brief 添加老师 的按钮被点击
     */

    connect(ui->listWidget_teacher, &QListWidget::itemDoubleClicked, [=](QListWidgetItem *item) {
        // 弹出对话框让用户修改文本
        bool ok;
        QString prefix = item->text().split(": ").first();
        QString name = item->text().split(": ").last();

        QString newText = QInputDialog::getText(this, tr("Edit Item"),
                                                tr("Enter new text:"),
                                                QLineEdit::Normal,
                                                name,
                                                &ok);
        if (ok && !newText.isEmpty()) {
            item->setText(prefix+": "+newText);
        }
    });

    // 删除老师 按钮
    QTimer pressLongTime;
    pressLongTime.setSingleShot(true); //开启触发信号
    pressLongTime.setInterval(1000); //设置间隔
    connect(ui->bt_delTeacher, &QPushButton::clicked, [=]() {
        QListWidgetItem *item = ui->listWidget_teacher->currentItem();

        if (item) {
            // 获取选中项的行号
            int row = ui->listWidget_teacher->row(item);

            // 删除选中的项
            delete item;

            // 更新剩余项的前缀编号
            int num = 1;
            for (int i = 0; i < ui->listWidget_teacher->count(); ++i) {
                QListWidgetItem *currentItem = ui->listWidget_teacher->item(i);
                if (currentItem) {
                    currentItem->setText(QString("老师%1 : %2").arg(num++).arg(currentItem->text().split(": ").last()));
                }
            }
        } else {
            // 没有选中项，删除最后一个项
            int count = ui->listWidget_teacher->count();
            if (count > 0) {
                delete ui->listWidget_teacher->item(count - 1);
            }
        }
    });

    connect(ui->spinBox_7,&QSpinBox::valueChanged,this,[=](int val){
        ui->spinBox_8->setValue(10-val);
    });
    connect(ui->spinBox_8,&QSpinBox::valueChanged,this,[=](int val){
        ui->spinBox_7->setValue(10-val);
    });
    connect(ui->spinBox_classHours,&QSpinBox::valueChanged,this,[=](int val){
        ui->spinBox_classNum->setValue(val/2);
    });
}


NewFileDialog::~NewFileDialog()
{
    delete ui;
}

/**
 * @brief NewFileDialog::on_pushButton_addTeacher_clicked 添加教师被点击
 * 点击按钮 listwidget 中添加一个 项
 */
void NewFileDialog::on_bt_addTeacher_clicked()
{
    int nums = ui->listWidget_teacher->count();

    // 创建新的 QListWidgetItem
    QListWidgetItem *newItem = new QListWidgetItem(QString("教师%1 : ").arg(nums+1));

    // 设置字体样式
    QFont font;
    font.setPointSize(14); // 设置与样式表中相同的字体大小
    newItem->setFont(font);

    // 添加新项到 listWidget
    ui->listWidget_teacher->addItem(newItem);
}



/**
 * @brief NewFileDialog::on_bt_check_clicked
 * 点击时 将 文本框中 录入 基本信息 进行检查对比
 */
void NewFileDialog::on_bt_check_clicked()
{
    QString result = "NULL _Information_";

    //课程id 和 课程名称
    courseData.classID = QVariant(ui->lineEdit_courseID->text());
    courseData.className = QVariant(ui->lineEdit_courseName->text());
    courseData.classType = QVariant(ui->comboBox_courseType->currentText());

    //拼接教师字符串
    int size_list = ui->listWidget_teacher->count();
    QString t_tea_name = "";
    for(int i = 0;i < size_list;++ i){
        t_tea_name.append(ui->listWidget_teacher->item(i)->text().remove(0,6)).append("、");
    }
    courseData.teacher_name = t_tea_name.remove(t_tea_name.size()-1,1);

    //课时 和 学分 上课次数
    courseData.lessonTime = QVariant(ui->spinBox_classHours->value());
    courseData.credit = QVariant(ui->spinBox_Credit->value());
    courseData.number_of_classes = QVariant(ui->spinBox_classNum->value());

    //学年\学期
    courseData.shoolDays = ui->radioButton->isChecked() ? QVariant(1) : QVariant(2);
    QString t_schoolYears = "";
    t_schoolYears = QString("%1 - %2 学年第 %3 学期")
                        .arg(ui->spinBox->value())
                        .arg(ui->spinBox_2->value())
                        .arg(courseData.shoolDays.toInt());

    courseData.schoolYears = QVariant(t_schoolYears);


    result = QString(
        R"( 学年: %1
            教师: %2
            课程名称: %3  课程id: %4  课程类型: %5
            课时: %6  学分: %7 上课次数: %8
        )"
        ).arg(courseData.schoolYears.toString())
        .arg(courseData.teacher_name.toString())
        .arg(courseData.className.toString()).arg(courseData.classID.toString()).arg(courseData.classType.toString())
        .arg(courseData.lessonTime.toString()).arg(courseData.credit.toString()).arg(courseData.number_of_classes.toString());
    ui->textEdit_bottom->setText(result);

}

/**
 * @brief NewFileDialog::getCourseData 课程信息get、set方法
 * @return 课程信息
 */
NewFileDialog::CourseData NewFileDialog::getCourseData() const
{
    return courseData;
}

void NewFileDialog::setCourseData(const CourseData &newCourseData)
{
    courseData = newCourseData;
}
/**
 * @brief NewFileDialog::on_bt_ok_clicked
 * 确定按钮被点击 将 信息 录入到 finalsheet 中，然后在 mainwidget 中创建表
 */
void NewFileDialog::on_bt_ok_clicked()
{
    //先通过 信号和 槽传出 数据

    //做验证，查看是否成功传输成功

    //关闭 该页面
    this->close();
    // 触发信号 创建 表
}

