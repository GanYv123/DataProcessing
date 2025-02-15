#include "newfiledialog.h"
#include "ui_newfiledialog.h"
#include "QInputDialog"
#include "QTimer"
#include "QFileDialog"
#include "QUrl"
#include "QStandardPaths"


NewFileDialog::NewFileDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewFileDialog)
{
    ui->setupUi(this);
    connectSignalANDSlot();
    buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->radioButton);
    buttonGroup->addButton(ui->radioButton_2);
    ui->label_15->setText(QString("缺勤扣分%1").arg(ui->horizontalSlider->value(), 2, 10, QLatin1Char('0')));
    ui->tabWidget->setCurrentIndex(0);

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
                    currentItem->setText(QString("教师%1 : %2").arg(num++).arg(currentItem->text().split(": ").last()));
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

    connect(ui->spinBox_attendance,&QSpinBox::valueChanged,this,[=](int val){
        ui->spinBox_experiment->setValue(10-(ui->spinBox_homework->value()+val));
    });
    connect(ui->spinBox_homework,&QSpinBox::valueChanged,this,[=](int val){
        ui->spinBox_experiment->setValue(10-(val+ui->spinBox_attendance->value()));
    });

    DragDropLabel *dragDropLabel;
    dragDropLabel =  new DragDropLabel(this);
    ui->widget_Droglabel->layout()->addWidget(dragDropLabel);
    connect(dragDropLabel,&DragDropLabel::sendFilePath,this,&NewFileDialog::handle_sendFilePath);

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

    //专业名称
    courseData.major = QVariant(ui->lineEdit->text());

    //考核分数比列
    courseData.rate_attendance = QVariant(ui->spinBox_attendance->value());
    courseData.rate_experiment = QVariant(ui->spinBox_experiment->value());
    courseData.rate_homework = QVariant(ui->spinBox_homework->value());

    //扣分
    courseData.attendance_reduce_fractions = QVariant(ui->horizontalSlider->value());

    result = QString(
    R"( 学年: %1  教师: %2
        课程名称: %3  课程id: %4  课程类型: %5
        课时: %6  学分: %7 上课次数: %8
        专业名称: %9
        考核比例: 考勤:%10% - 实验:%11% - 作业:%12%
        考勤扣分:%13
    )"
                 ).arg(courseData.schoolYears.toString())
                 .arg(courseData.teacher_name.toString())
                 .arg(courseData.className.toString())
                 .arg(courseData.classID.toString())
                 .arg(courseData.classType.toString())
                 .arg(courseData.lessonTime.toString())
                 .arg(courseData.credit.toString())
                 .arg(courseData.number_of_classes.toString())
                 .arg(courseData.major.toString())
                 .arg(courseData.rate_attendance.toInt() *10)
                 .arg(courseData.rate_experiment.toInt() *10)
                 .arg(courseData.rate_homework.toInt() *10).arg(courseData.attendance_reduce_fractions.toString());
    ui->textEdit_bottom->clear();
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
 * 信号和槽传输数据 目前暂定为传输 finalsheet::CourseData
 */
void NewFileDialog::on_bt_ok_clicked()
{
    emit ui->bt_check->clicked();

    // 创建一个局部事件循环
    QEventLoop loop;
    QTimer::singleShot(500, &loop, SLOT(quit())); // 0.5秒后退出事件循环
    loop.exec(); // 进入事件循环，阻塞当前代码继续执行


    //先将临时的数据 创建和赋值到 一个 finalSheet::courseData 对象里面
    FinalSheet::CourseData t_courseData;

    t_courseData.teacher_name = courseData.teacher_name;
    t_courseData.shoolDays = courseData.shoolDays;
    t_courseData.major = courseData.major;
    t_courseData.classID = courseData.classID;
    t_courseData.classType = courseData.classType;
    t_courseData.number_of_classes = courseData.number_of_classes;
    t_courseData.className = courseData.className;
    t_courseData.rate_attendance = courseData.rate_attendance;
    t_courseData.rate_homework = courseData.rate_homework;
    t_courseData.rate_experiment = courseData.rate_experiment;
    t_courseData.attendance_reduce_fractions = courseData.attendance_reduce_fractions;
    t_courseData.lessonTime = courseData.lessonTime;
    t_courseData.credit = courseData.credit;
    t_courseData.schoolYears = courseData.schoolYears;


    //先通过 信号和 槽传出 数据
    emit sendCourseData(t_courseData);
    //做验证，查看是否成功传输成功

    //关闭 该页面
    this->close();
    // 触发信号 创建 表
}

/**
 * @brief NewFileDialog::on_spinBox_valueChanged
 * 当改变值时调整 拎另一个学年的选择变化，跨度为1
 * @param arg1 变化前的值
 */
void NewFileDialog::on_spinBox_valueChanged(int arg1)
{
    ui->spinBox_2->setValue(arg1+1);
}
void NewFileDialog::on_spinBox_2_valueChanged(int arg1)
{
    ui->spinBox->setValue(arg1-1);
}

/**
 * @brief NewFileDialog::on_horizontalSlider_valueChanged
 * 扣分滑条移动时 改变标签 15 显示
 * @param value
 */
void NewFileDialog::on_horizontalSlider_valueChanged(int value)
{
    ui->label_15->setText(QString("缺勤扣分%1").arg(value, 2, 10, QLatin1Char('0')));
}

void NewFileDialog::updateStudentInfo(const QString &name, const QString &studentID,
                                      const QString &major, const QString &enrollmentDate,
                                      const QString &iconPath)
{
    // 构造新的 HTML 内容来更新 QLabel
    QString htmlText = QString("<html>"
                               "<head/>"
                               "<body>"
                               "<table border=\"0\" style=\"margin:0px;\" cellspacing=\"2\" cellpadding=\"0\">"
                               "<tr>"
                               "<td><p><span style=\"font-weight:700;\">姓名:</span></p><p><br/></p></td>"
                               "<td><p>%1</p></td>"
                               "<td rowspan=\"4\" style=\"text-align:center; vertical-align:middle;\">"
                               "<img src=\"%5\" style=\"width:100px; height:100px; object-fit:contain;\"/>"
                               "</td>"
                               "</tr>"
                               "<tr>"
                               "<td><p><span style=\"font-weight:700;\">学号:</span></p><p><br/></p></td>"
                               "<td><p>%2</p></td>"
                               "</tr>"
                               "<tr>"
                               "<td><p><span style=\"font-weight:700;\">专业:</span></p><p><br/></p></td>"
                               "<td><p>%3</p></td>"
                               "</tr>"
                               "<tr>"
                               "<td><p><span style=\"font-weight:700;\">入学年:</span></p></td>"
                               "<td><p>%4</p><p><br/></p></td>"
                               "</tr>"
                               "</table>"
                               "</body>"
                               "</html>").arg(name).arg(studentID).arg(major).arg(enrollmentDate).arg(iconPath);

    // 更新 QLabel 的文本内容
    ui->label_markdown->setText(htmlText);
}





/**
 * @brief NewFileDialog::on_listWidget_itemClicked
 * 班级1学生信息item 被点击 更新 html 文本
 * @param item
 */
void NewFileDialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    qDebug() << "一班学生信息:" << item->text();
    updateStudentInfo(item->text(), "", "", "", "");
}

/**
 * @brief NewFileDialog::on_listWidget_itemClicked
 * 班级2学生信息item 被点击
 * @param item
 */

void NewFileDialog::on_listWidget_2_itemClicked(QListWidgetItem *item)
{
    qDebug()<<"二班学生信息:"<<item->text();
    updateStudentInfo(item->text(), "", "", "","");
}

void NewFileDialog::handle_sendFilePath(QString filePath)
{
    filePath_stuInfors = filePath;
    qDebug()<<"获取到文件地址"<<filePath_stuInfors;
    //获取到文件则读取表
    bool ret = false;
    QMap<QString, QList<QVariant>> stus_IdName = operExcel->getStudentNameAndId(filePath_stuInfors,ret,this);
    if(ret){
        for (auto it = stus_IdName.begin(); it != stus_IdName.end(); ++it)
        {
            QList<QVariant> nameAndClass_id = it.value();
            qDebug() << it.key() << nameAndClass_id.at(0) << nameAndClass_id.at(1);
        }


    }else{
        qDebug()<<"读取错误！";
    }

}

//-----------------------------自定义label
DragDropLabel::DragDropLabel(QWidget *parent) : QLabel(parent)
{
    setAcceptDrops(true); // 允许接收拖拽事件
    setTextFormat(Qt::RichText); // 设置文本格式为富文本
    setAlignment(Qt::AlignCenter); // 文本居中显示
    updateLabelText(); // 更新初始文本
}

void DragDropLabel::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()){
        QUrl filePath = event->mimeData()->urls().at(0);
        QString path = filePath.path();
        QFileInfo fileInfo(path);
        QString suffix = fileInfo.suffix();
        setText(QString("file type : %1").arg(suffix));
        // 根据文件后缀名执行不同的操作

        if (!suffix.isEmpty()) {
            if (suffix == "txt" || suffix == "csv") {
                qDebug() << "Text file or CSV file detected.";

                event->acceptProposedAction();
            } else if (suffix == "xlsx" || suffix == "xls" || suffix == "xlsm") {
                qDebug() << "Excel file detected.";

                event->acceptProposedAction();
            } else if (suffix == "pdf") {
                qDebug() << "PDF file detected.";

                event->acceptProposedAction();
            } else if(suffix == "ini") {
                qDebug()<<"导入的是配置文件:";
                 event->acceptProposedAction();
            }
            else {

                // TODO: 处理其他不支持的文件类型的逻辑
                event->ignore();
            }
        } else {
            qDebug() << "No file extension detected.";

        }
    }
}

/**
 * @brief DragDropLabel::dropEvent
 * 放置上去时 拿到文件地址
 * @param event
 */
void DragDropLabel::dropEvent(QDropEvent *event)
{
    emit sendFilePath(event->mimeData()->urls().at(0).toString().mid(8));
}

/**
 * @brief DragDropLabel::mouseDoubleClickEvent
 * 鼠标双击label 事件
 * @param event
 */
void DragDropLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        double_clicked_label();
    }else if (event->button() == Qt::RightButton){
        qDebug()<<"右键 双击! 打开数据库连接 导入数据";
        //调用 连接数据库的 对话框
    }
}

//离开时 重置显示文本
void DragDropLabel::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
    updateLabelText();
}

void DragDropLabel::updateLabelText()
{
    // 初始文本，显示拖拽文件的提示信息
    setText("<p style='font-size:14px;'>从文件中批量导入学生信息</p>"
            "<p>将文件拖拽到此处上传</p>"
            "<p style='font-size:15px;'>左键双击打开文件选择窗口</p>"
            "<p style='font-size:15px;'>右键双击连接数据库</p>");
}
/**
 * @brief DragDropLabel::double_clicked_label
 * 双击 打开文件选择对话框
 */
void DragDropLabel::double_clicked_label()
{
    qDebug()<<"双击!打开文件选择对话框";
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Images (*.xlsx *.pdf *.txt *.csv)"));
    if(!filePath.isEmpty())
        emit sendFilePath(filePath);
}
