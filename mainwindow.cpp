#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QLabel"
#include "QFile"
#include "QSize"
#include "QUrl"
#include "QFileInfo"
#include "QFileDialog"
#include "operexcel.h"
#include "QComboBox"
#include "QAction"
#include "QWidgetAction"
#include "QStandardItemModel"
#include "QStandardItem"
#include "QInputDialog"
#include "finalsheet.h"
#include "QMessageBox"
#include "iostream"
#include "mysettings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),finalSheet(new FinalSheet(this))
{
    ui->setupUi(this);

    this->setAcceptDrops(true);
    ui->tableView->setAcceptDrops(false);
    this->customDialog = new CustomDialog(this);
    this->customDialog->setAcceptDrops(false);
    customDialog->close();
    operExcel = new OperExcel(this,finalSheet);

    initMainWindow();
    connect(this,&MainWindow::student_added,this,&MainWindow::slots_student_added);
    connect(table_model1,&QStandardItemModel::itemChanged,this,&MainWindow::handleItemChanged1);
    connect(table_model2,&QStandardItemModel::itemChanged,this,&MainWindow::handleItemChanged2);
}

/**
 * 析构
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    if(ui)
        delete ui;
    //设置配置文件为隐藏

}
/**
 * @brief MainWindow::initMainWindow 初始化界面组件
 */
void MainWindow::initMainWindow()
{//初始化界面组件
    ui->ac_hidden_configFile->setChecked(MySettings::instance().getIshided());
    label_tips = new QLabel("状态栏pass",this);
    ui->statusbar->addWidget(label_tips);
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    label_size = new QLabel("size: x , y",this);
    label_CourseInfo = new QLabel(" Null");
    ui->statusbar->addWidget(new QWidget(),1);
    ui->statusbar->addPermanentWidget(label_CourseInfo);
    ui->statusbar->addPermanentWidget(label_size);

    ui->tableView->move(20,20);
    ui->tableView->resize(this->width()-40,this->height()-120);
    this->setMinimumSize(500,400);

    //初始化tableView
    if(table_model1 == nullptr)
        table_model1 = new QStandardItemModel(this);
    if(table_model2 == nullptr)
        table_model2 = new QStandardItemModel(this);

    QStringList heardLabels;
    heardLabels<<"学号"<<"姓名"<<"考勤"<<"作业"<<"实验"<<"总成绩"<<"备注";

    table_model1->setHorizontalHeaderLabels(heardLabels);
    table_model2->setHorizontalHeaderLabels(heardLabels);

    //需要有优化逻辑
    ui->tableView->setModel(table_model1);
    ui->tableView_2->setModel(table_model2);

    ui->tabWidget->setCurrentIndex(0); //设置默认选项卡
    // 设置 QTabWidget 的样式表
    ui->tabWidget->setStyleSheet(
        "QTabBar::tab:selected {"
        "   background-color: #99eb1f; /* 设置选中时的背景颜色 */"
        "}"
        );


    // 获取表格模型并尝试转换为 QStandardItemModel
    QStandardItemModel *tableModel = qobject_cast<QStandardItemModel *>(ui->tableView->model());
    if (tableModel) {
        operExcel->setViewModel(tableModel); // 如果转换成功，则传递给 OperExcel
    } else {
        // 处理转换失败的情况
        qDebug() << "Failed to cast table model to QStandardItemModel";
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) //拖拽进入
{
    qDebug()<<event->mimeData()->urls();
    if(event->mimeData()->hasUrls()){
        filePath = event->mimeData()->urls().at(0);
        QString path = filePath.path();
        QFileInfo fileInfo(path);
        QString suffix = fileInfo.suffix();
        this->label_tips->setText(QString("file type : %1").arg(suffix));
        // 根据文件后缀名执行不同的操作

        if (!suffix.isEmpty()) {
            if (suffix == "txt" || suffix == "csv") {
                qDebug() << "Text file or CSV file detected.";

                // TODO: 处理文本文件或 CSV 文件的逻辑
                event->acceptProposedAction();
            } else if (suffix == "xlsx" || suffix == "xls" || suffix == "xlsm") {
                qDebug() << "Excel file detected.";
                this->label_tips->setText("可接收拖入文件");
                event->acceptProposedAction();
            } else if (suffix == "pdf") {
                qDebug() << "PDF file detected.";

                event->acceptProposedAction();
            } else {

                // TODO: 处理其他不支持的文件类型的逻辑
                event->ignore();
            }
        } else {
            qDebug() << "No file extension detected.";

        }
    }
}

void MainWindow::dropEvent(QDropEvent *event) //放入时
{
    //显示表格到tableView
    handleFile(event->mimeData()->urls().at(0).toString().mid(8));
}

void MainWindow::resizeEvent(QResizeEvent *event) //尺寸改变
{
    QSize sz = this->size();
    ui->tableView->resize(sz.width()-40,sz.height()-120);
    this->label_size->setText(QString("size: %1 , %2").arg(sz.width()).arg(sz.height()));
    event->accept();//事件接收
}

void MainWindow::handleFile(const QString &filePath)
{//处理文件
    this->setWindowTitle("已打开文件");
    this->setAcceptDrops(false);
    // 在这里实现对文件的具体处理逻辑
    if(!operExcel)
        operExcel = new OperExcel(this);

    path = filePath;
    //path.replace("\\","/");
    qDebug() << "Handling file: " << path;

    bool ret = false;
    operExcel->open_Excel(path,ret,this);
    if(ret){
        this->label_tips->setText("文件打开成功!");
        qInfo()<<"文件打开成功！";

        //考勤信息初始化
        if(table_attdendance == nullptr)
        {
            table_attdendance = new QStandardItemModel;
            QStringList heardLabels;
            heardLabels<<"学号1"<<"姓名1"<<"1考勤次数"<<"学号2"<<"姓名2"<<"2考勤次数";
            table_attdendance->setHorizontalHeaderLabels(heardLabels);
            operExcel->setAttdendanceViewModel(table_attdendance);
            connect(table_attdendance,&QStandardItemModel::itemChanged,this,
                    &MainWindow::handleItemChanged_attendance);
        }
        ui->tableView_3->setModel(table_attdendance);

        QStringList heardLaber;
        heardLaber<<"学号"<<"姓名";
        if(table_experiment1 == nullptr){
            table_experiment1 = new QStandardItemModel(this);
            table_experiment1->setHorizontalHeaderLabels(heardLaber);

        }
        if(table_experiment2 == nullptr){
            table_experiment2 = new QStandardItemModel(this);
            table_experiment2->setHorizontalHeaderLabels(heardLaber);
        }
        operExcel->setExperimentViewModel(table_experiment1,1);
        operExcel->setExperimentViewModel(table_experiment2,2);

        if(table_homeWork1 == nullptr){
            table_homeWork1 = new QStandardItemModel(this);
            table_homeWork1->setHorizontalHeaderLabels(heardLaber);
        }
        if(table_homeWork2 == nullptr){
            table_homeWork2 = new QStandardItemModel(this);
            table_homeWork2->setHorizontalHeaderLabels(heardLaber);
        }
        operExcel->setHomeWorkViewModel(table_homeWork1,1);
        operExcel->setHomeWorkViewModel(table_homeWork2,2);

    }else{
        this->label_tips->setText("文件未打开");
        qWarning()<<"文件未打开!";
    }

}

void MainWindow::setLabel_CourseInfo(const QString &text)
{
    this->label_CourseInfo->setText(text);
}

QStandardItemModel *MainWindow::getClass1Model()
{
    return this->table_model1;
}

QStandardItemModel *MainWindow::getClass2Model()
{
    return this->table_model2;
}

void MainWindow::on_ac_openFiles_triggered() //打开文件
{
    //创建文件选择对话框
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Excel Files (*.xlsx *.xls)");
    // 显示文件选择对话框
    if (dialog.exec()) {
        // 获取选中的文件路径（只有一个文件）
        QString selectedFile = dialog.selectedFiles().at(0);

        qDebug() << "Selected file: " << selectedFile;

        // 在这里可以进行文件的处理，比如读取文件内容等
        handleFile(selectedFile);
    }
}


//增加try catch 日志记录功能
void MainWindow::on_ac_creatFiles_triggered() //demo function BETA 1
{//新建文件 自己导入空文件 学生信息都空！
    if(!operExcel)
        operExcel = new OperExcel();
    bool ret;
    QString path = "./1.xlsx";

}

void MainWindow::on_ac_choose_school_year_triggered()
{//下拉框选择学年
    if(customDialog == nullptr)
        this->customDialog = new CustomDialog(this);
    customDialog->move(this->width()/2-150,this->height()/2-100);
    customDialog->resize(300,200);
    customDialog->show_choose_schoolYears();

}

/**
 * @warning 禁用函数
 * @brief MainWindow::on_tableView_doubleClicked
 * @param index
 */
void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    //禁用函数
    Q_UNUSED(index);
    return;
}


void MainWindow::on_ac_saveFiles_triggered()
{//保存文件
    bool ret = false;
    operExcel->save_Excel(path,ret,this);
    if(ret){
        qDebug()<<"保存成功";
        this->label_tips->setText("文件保存成功");
    }else{
        qDebug()<<"保存失败";
        this->label_tips->setText("保存失败");
    }
}

void MainWindow::on_ac_exportExcel_triggered()
{//导出表格
    if(this->path == "NullPath" && notConfig)
    {
        qWarning()<<"未导入文件";
        showMessageBox("未导入文件!");
        return;
    }

    QString saveFilePath = QFileDialog::getSaveFileName
        (this, tr("选择保存位置和文件名"), QDir::currentPath(), tr("Excel files (*.xlsx)"));
    if (saveFilePath.isEmpty()) {
        return;
    }

    bool ret;
    this->operExcel->export_Excel(saveFilePath, ret, this);
    if(ret){
        qDebug()<<"导出成功";
        showMessageBox("导出成功! ");
    } else {
        qDebug()<<"导出失败";
        showMessageBox("导出失败，请查看是在打开"+saveFilePath+"时操作! ");
    }
}

/**
@test 待修改函数
@title 演示函数
*/
void MainWindow::on_ac_addStu_triggered()
{//添加学生 得到name id classid
    if(this->path == "NullPath" && this->notConfig)
    {
        qWarning()<<"未导入文件|配置";
        this->label_tips->setText("未导入文件|配置");
        return;
    }

    StudentDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString name = dialog.getStudentName();
        QString id = dialog.getStudentID();
        QString className = dialog.getClassName();

        QList<QVariant*> list;
        list << new QVariant(name);
        list << new QVariant(id);
        list << new QVariant(className);

        emit student_added(list);
    }
}

/**
 * @brief MainWindow::slots_student_added
 * @param itemList
 * @todo 待修改函数
 */
void MainWindow::slots_student_added(QList<QVariant*> list) {
    // 获取完信息添加到 m_fin 的对应班级表中
    if(list.last()->toInt() == 1) {
        qDebug() << "1班学生" << *list.at(0) << " " << *list.at(1);
        QVector <FinalSheet::StudentData> stu_1 = finalSheet->class1_students();
        FinalSheet::StudentData t_stu;
        t_stu.studentName = QVariant(*list.at(0));
        t_stu.studentID = QVariant(*list.at(1));
        t_stu.sub_experiment.resize(stu_1.at(0).sub_experiment.size());
        t_stu.sub_homework.resize(stu_1.at(0).sub_homework.size());
        stu_1.insert(0,t_stu);
        finalSheet->setClass1Students(stu_1);

        // 暂时禁用信号和槽的连接
        table_model1->blockSignals(true);
        table_experiment1->blockSignals(true);
        table_homeWork1->blockSignals(true);

        // 删除所有行，但保留表头
        table_model1->removeRows(0, table_model1->rowCount());
        operExcel->setClassTableViewModel(table_model1, 1);
        table_experiment1->removeRows(0, table_experiment1->rowCount());
        operExcel->setExperimentViewModel(table_experiment1, 1);
        table_homeWork1->removeRows(0, table_homeWork1->rowCount());
        operExcel->setHomeWorkViewModel(table_homeWork1, 1);

        // 恢复信号和槽的连接
        table_model1->blockSignals(false);
        table_experiment1->blockSignals(false);
        table_homeWork1->blockSignals(false);
    }

    if(list.last()->toInt() == 2) {
        qDebug() << "2班学生" << *list.at(0) << " " << *list.at(1);
        QVector <FinalSheet::StudentData> stu_2 = finalSheet->class2_students();
        FinalSheet::StudentData t_stu;
        t_stu.studentName = QVariant(*list.at(0));
        t_stu.studentID = QVariant(*list.at(1));
        t_stu.sub_experiment.resize(stu_2.at(0).sub_experiment.size());
        t_stu.sub_homework.resize(stu_2.at(0).sub_homework.size());
        stu_2.insert(0,t_stu);
        finalSheet->setClass2Students(stu_2);

        // 暂时禁用信号和槽的连接
        table_model2->blockSignals(true);
        table_experiment2->blockSignals(true);
        table_homeWork2->blockSignals(true);

        // 删除所有行，但保留表头
        table_model2->removeRows(0, table_model2->rowCount());
        operExcel->setClassTableViewModel(table_model2, 2);
        table_experiment2->removeRows(0, table_experiment2->rowCount());
        operExcel->setExperimentViewModel(table_experiment2, 2);
        table_homeWork2->removeRows(0, table_homeWork2->rowCount());
        operExcel->setHomeWorkViewModel(table_homeWork2, 2);

        // 恢复信号和槽的连接
        table_model2->blockSignals(false);
        table_experiment2->blockSignals(false);
        table_homeWork2->blockSignals(false);
    }

    // 暂时禁用信号和槽的连接
    table_attdendance->blockSignals(true);

    // 刷新考勤表
    this->table_attdendance->removeColumns(0, table_attdendance->rowCount());
    operExcel->setAttdendanceViewModel(table_attdendance);

    // 恢复信号和槽的连接
    table_attdendance->blockSignals(false);
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if (index.isValid()) {
        // 获取单击的单元格的行和列索引
        int row = index.row();
        int column = index.column();

        // 获取单击的单元格的数据
        QVariant cellData = index.data(Qt::DisplayRole);

        // 根据需要执行其他操作，例如显示信息
        qDebug() << "Clicked cell at row:" << row << ", column:" << column << ", data:" << cellData.toString();

    }
}

/**
 * @warning 弃用的函数 之前用于选择班级
*/
void MainWindow::on_ac_checkMajor_triggered()
{//点击选择的班级 切换 不同班级的 model

    // FinalSheet::CourseData t_courseData = finalSheet->getCourseData();
    // if(t_courseData.classID.isNull())
    // {
    //     qWarning()<<"未导入文件\\未读取到信息";
    //     this->label_tips->setText("未导入文件");
    //     showMessageBox("请先导入文件");
    //     return;
    // }
    // this->customDialog_chooseClassID =
    //     new CustomDialog_chooseClassID(t_courseData.major.toString().append(" - 1"),
    //                                    t_courseData.major.toString().append(" - 2"),this);
    // customDialog_chooseClassID->move(this->width()/2-150,this->height()/2-100);
    // customDialog_chooseClassID->resize(300,200);
    // customDialog_chooseClassID->show_chooseClassID();
    // //根据选择班级切换模型
    // if(customDialog_chooseClassID->get_select_data().contains("- 1")){
    //     qDebug()<<"选择了班级1";
    //     this->currentChooseClassID = 1;
    //     ui->tableView->setModel(this->table_model1);
    //     this->label_tips->setText(finalSheet->getCourseData().major.toString().append(" 1 班"));
    // }else if(customDialog_chooseClassID->get_select_data().contains("- 2")){
    //     qDebug()<<"选择了班级2";
    //     this->currentChooseClassID = 2;
    //     ui->tableView->setModel(this->table_model2);
    //     this->label_tips->setText(finalSheet->getCourseData().major.toString().append(" 2 班"));
    // }else{
    //     qDebug()<<"unknow class";
    // }
}

/**
 * @brief MainWindow::on_ac_Attendance_triggered
 * @todo 迁移了原来的实现函数 将用作 显示详细的考勤信息
 */
void MainWindow::on_ac_Attendance_triggered()
{//需要单独创建一个model来存储考勤信息
    if(path == "NullPath" &&  notConfig)
    {
        showMessageBox("请先导入文件");
        return;
    }
    //迁移函数到 初始化函数 该页面用作 详细成绩显示

}


void MainWindow::handleItemChanged1(QStandardItem *item)
{
    // 获取发生更改的项的索引
    QModelIndex changedIndex = item->index();
    int row = changedIndex.row();

    // 获取对应的数据项
    QVector<FinalSheet::StudentData> class1Students = finalSheet->class1_students();
    FinalSheet::StudentData studentData = class1Students[row];

    // 根据列号修改对应的数据
    if (item->column() == 0) { // 学生ID
        studentData.studentID = item->text();
    } else if (item->column() == 1) { // 学生姓名
        studentData.studentName = item->text();
    } else if (item->column() == 2) { // 考勤成绩
        studentData.attendanceScore = item->text();
    } else if (item->column() == 3) { // 作业成绩
        studentData.homework = item->text();
    } else if (item->column() == 4) { // 实验成绩
        studentData.experiment = item->text();
    } else if (item->column() == 6) { // 备注
        studentData.remark = item->text();
    }
    // 继续根据需要处理其他列
    // 更新对应的数据项
    FinalSheet::CourseData courseData = finalSheet->getCourseData();

    QVariant attdance = QVariant((table_model1->item(row,2)->text()));
    QVariant homework =  QVariant((table_model1->item(row,3)->text()));
    QVariant experiment =  QVariant((table_model1->item(row,4)->text()));
    double t_total = ( (attdance.toDouble() * courseData.rate_attendance.toDouble()/100.0) +
                      (homework.toDouble() * courseData.rate_homework.toDouble()/100.0) +
                      (experiment.toDouble() * courseData.rate_experiment.toDouble()/100.0));
    //四舍五入取整
    QVariant total = QVariant(static_cast<int>(std::round(t_total)));
    QStandardItem* t_item = table_model1->item(row,5);
    t_item->setText(total.toString());

    class1Students[row] = studentData;

    // 通过 FinalSheet 更新数据
    finalSheet->setClass1Students(class1Students);
    //刷新model1
    operExcel->countTotalScore();
    ui->tableView->viewport()->update();
}


void MainWindow::handleItemChanged2(QStandardItem *item)
{
    // 获取发生更改的项的索引
    QModelIndex changedIndex = item->index();
    int row = changedIndex.row();

    // 获取对应的数据项
    QVector<FinalSheet::StudentData> class2Students = finalSheet->class2_students();
    FinalSheet::StudentData studentData = class2Students[row];

    // 根据列号修改对应的数据
    if (item->column() == 0) { // 学生ID
        studentData.studentID = item->text();
    } else if (item->column() == 1) { // 学生姓名
        studentData.studentName = item->text();
    } else if (item->column() == 2) { // 考勤成绩
        studentData.attendanceScore = item->text();
    } else if (item->column() == 3) { // 作业成绩
        studentData.homework = item->text();
    } else if (item->column() == 4) { // 实验成绩
        studentData.experiment = item->text();
    } else if (item->column() == 6) { // 备注
        studentData.remark = item->text();
    }

    // 继续根据需要处理其他列

    // 更新对应的数据项

    FinalSheet::CourseData courseData = finalSheet->getCourseData();

    QVariant attdance = QVariant((table_model2->item(row,2)->text()));
    QVariant homework =  QVariant((table_model2->item(row,3)->text()));
    QVariant experiment =  QVariant((table_model2->item(row,4)->text()));

    double t_total = ( (attdance.toDouble() * courseData.rate_attendance.toDouble()/100.0) +
                      (homework.toDouble() * courseData.rate_homework.toDouble()/100.0) +
                      (experiment.toDouble() * courseData.rate_experiment.toDouble()/100.0));
    //四舍五入取整
    QVariant total = QVariant(static_cast<int>(std::round(t_total)));

    QStandardItem* t_item = table_model2->item(row,5);
    t_item->setText(total.toString());

    class2Students[row] = studentData;

    // 通过 FinalSheet 更新数据
    finalSheet->setClass2Students(class2Students);
    //刷新model2
    operExcel->countTotalScore();
    ui->tableView_2->viewport()->update();
}

/**
 * @brief MainWindow::handleItemChanged_attendance
 * @param item 槽函数 处理考勤表改变的信息
 */

void MainWindow::handleItemChanged_attendance(QStandardItem *item)
{
    // 获取发生更改的项的索引
    QModelIndex changedIndex = item->index();
    int row = changedIndex.row();
    int col = changedIndex.column();
    const int class1Index = 2,class2Index = 5;

    qDebug()<<"row:"<<row<<"  col"<<col;

    if (col == class1Index) {
        // 获取对应的数据项
        QVector<FinalSheet::StudentData> class1Students = finalSheet->class1_students();

        // 检查 row 是否越界
        if (row < 0 || row >= class1Students.size()) {
            QMessageBox::critical(nullptr, "Error", "Row index out of range for class1Students.");
            item->setText("");
            return;
        }

        FinalSheet::StudentData studentData = class1Students.at(row);

        // 检查 item 是否为 nullptr
        if (item == nullptr) {
            QMessageBox::critical(nullptr, "Error", "Item is a nullptr.");
            item->setText("");
            return;
        }

        studentData.attendance = QVariant(item->text());
        class1Students[row] = studentData;

        // 同步修改到 finalSheet
        int attScore = (finalSheet->getCourseData().attendance_reduce_fractions.toInt() *
                        (finalSheet->getCourseData().lessonTime.toInt() / 2 -
                         class1Students.at(row).attendance.toInt()));
        class1Students[row].attendanceScore = 100 - attScore;

        finalSheet->setClass1Students(class1Students);

        // 修改 model1 的 data
        this->table_model1->item(row, 2)->setText(finalSheet->class1_students().at(row).attendanceScore.toString());

        operExcel->countTotalScore();
        ui->tableView->viewport()->update();

    } else if (col == class2Index) {
        QVector<FinalSheet::StudentData> class2Students = finalSheet->class2_students();

        // 检查 row 是否越界
        if (row < 0 || row >= class2Students.size()) {
            QMessageBox::critical(nullptr, "Error", "Row index out of range for class2Students.");
            item->setText("");
            return;
        }

        FinalSheet::StudentData studentData = class2Students.at(row);

        // 检查 item 是否为 nullptr
        if (item == nullptr) {
            QMessageBox::critical(nullptr, "Error", "Item is a nullptr.");
            item->setText("");
            return;
        }

        studentData.attendance = QVariant(item->text());
        class2Students[row] = studentData;

        // 同步修改到 finalSheet
        int attScore = (finalSheet->getCourseData().attendance_reduce_fractions.toInt() *
                        (finalSheet->getCourseData().lessonTime.toInt() / 2 -
                         class2Students.at(row).attendance.toInt()));
        class2Students[row].attendanceScore = 100 - attScore;

        finalSheet->setClass2Students(class2Students);

        // 修改 model2 的 data
        this->table_model2->item(row, 2)->setText(finalSheet->class2_students().at(row).attendanceScore.toString());

        operExcel->countTotalScore();
        ui->tableView->viewport()->update();
    }

}

void MainWindow::handleItemChanged_experimentView1(QStandardItem *item)
{//实验成绩视图
    //qDebug()<<"实验1表被修改";
    disconnect(table_experiment1, &QStandardItemModel::itemChanged, this,
               &MainWindow::handleItemChanged_experimentView1);
    // 获取发生更改的项的索引
    QModelIndex changedIndex = item->index();
    int row = changedIndex.row();
    int col = changedIndex.column();
    // 获取对应的数据项
    QVector<FinalSheet::StudentData> class1Students = finalSheet->class1_students();
    FinalSheet::StudentData studentData = class1Students[row];

    if( col >= 2 && studentData.sub_experiment.size()-col+2 >= 0 && col < table_experiment1->columnCount()-1){
        studentData.sub_experiment[col-2] = QVariant(item->text().toInt());
        qDebug()<<"changed item : "<<studentData.sub_experiment[col-2];
    }
    //计算值变化后的平均成绩
    double mean_total = 0.0;
    for(const auto& a: studentData.sub_experiment){
        mean_total += a.toDouble();
    }
    mean_total = mean_total/studentData.sub_experiment.size();

    int res_mean = static_cast<int>(std::round(mean_total));
    studentData.experiment = QVariant(res_mean);
    QStandardItem* t_item_exp = table_experiment1->item(row,studentData.sub_experiment.size()+2);
    t_item_exp->setText(studentData.experiment.toString());

    class1Students[row] = studentData;
    finalSheet->setClass1Students(class1Students);
    //刷新model1
    operExcel->countTotalScore();
    ui->tableView->viewport()->update();
    QStandardItem* t_item_toal = table_model1->item(row,4);
    t_item_toal->setText(finalSheet->class1_students().at(row).experiment.toString());
    // 连接信号和槽
    connect(table_experiment1, &QStandardItemModel::itemChanged, this,
            &MainWindow::handleItemChanged_experimentView1);
}

void MainWindow::handleItemChanged_experimentView2(QStandardItem *item)
{
    //qDebug()<<"实验2表被修改";
    disconnect(table_experiment2, &QStandardItemModel::itemChanged, this,
               &MainWindow::handleItemChanged_experimentView2);
    // 获取发生更改的项的索引
    QModelIndex changedIndex = item->index();
    int row = changedIndex.row();
    int col = changedIndex.column();
    // 获取对应的数据项
    QVector<FinalSheet::StudentData> class2Students = finalSheet->class2_students();
    FinalSheet::StudentData studentData = class2Students[row];

    if( col >= 2 && studentData.sub_experiment.size()-col+2 >= 0 && col < table_experiment2->columnCount()-1){
        studentData.sub_experiment[col-2] = QVariant(item->text().toInt());
        qDebug()<<"changed item : "<<studentData.sub_experiment[col-2];
    }
    //计算值变化后的平均成绩
    double mean_total = 0.0;
    for(const auto& a: studentData.sub_experiment){
        mean_total += a.toDouble();
    }
    mean_total = mean_total/studentData.sub_experiment.size();

    int res_mean = static_cast<int>(std::round(mean_total));
    studentData.experiment = QVariant(res_mean);
    QStandardItem* t_item_exp = table_experiment2->item(row,studentData.sub_experiment.size()+2);
    t_item_exp->setText(studentData.experiment.toString());

    class2Students[row] = studentData;
    finalSheet->setClass2Students(class2Students);
    //刷新model1
    operExcel->countTotalScore();
    ui->tableView_2->viewport()->update();
    QStandardItem* t_item_toal = table_model2->item(row,4);
    t_item_toal->setText(finalSheet->class2_students().at(row).experiment.toString());
    // 连接信号和槽
    connect(table_experiment2, &QStandardItemModel::itemChanged, this,
            &MainWindow::handleItemChanged_experimentView2);
}

void MainWindow::handleItemChanged_homeworkView1(QStandardItem *item)
{
    disconnect(table_homeWork1, &QStandardItemModel::itemChanged, this,
               &MainWindow::handleItemChanged_homeworkView1);
    // 获取发生更改的项的索引
    QModelIndex changedIndex = item->index();
    int row = changedIndex.row();
    int col = changedIndex.column();
    // 获取对应的数据项
    QVector<FinalSheet::StudentData> class1Students = finalSheet->class1_students();
    FinalSheet::StudentData studentData = class1Students[row];

    if( col >= 2 && studentData.sub_homework.size()-col+2 >= 0 && col < table_homeWork1->columnCount()-1){
        studentData.sub_homework[col-2] = QVariant(item->text().toInt());
        qDebug()<<"changed item : "<<studentData.sub_homework[col-2];
    }
    //计算值变化后的平均成绩
    double mean_total = 0.0;
    for(const auto& a: studentData.sub_homework){
        mean_total += a.toDouble();
    }
    mean_total = mean_total/studentData.sub_homework.size();

    int res_mean = static_cast<int>(std::round(mean_total));
    studentData.homework = QVariant(res_mean);
    QStandardItem* t_item_hom = table_homeWork1->item(row,studentData.sub_homework.size()+2);
    t_item_hom->setText(studentData.homework.toString());

    class1Students[row] = studentData;
    finalSheet->setClass1Students(class1Students);
    //刷新model1
    operExcel->countTotalScore();
    ui->tableView->viewport()->update();
    QStandardItem* t_item_toal = table_model1->item(row,3);
    t_item_toal->setText(finalSheet->class1_students().at(row).homework.toString());
    // 连接信号和槽
    connect(table_homeWork1, &QStandardItemModel::itemChanged, this,
            &MainWindow::handleItemChanged_homeworkView1);
}

void MainWindow::handleItemChanged_homeworkView2(QStandardItem *item)
{
    disconnect(table_homeWork2, &QStandardItemModel::itemChanged, this,
               &MainWindow::handleItemChanged_homeworkView2);
    // 获取发生更改的项的索引
    QModelIndex changedIndex = item->index();
    int row = changedIndex.row();
    int col = changedIndex.column();
    // 获取对应的数据项
    QVector<FinalSheet::StudentData> class2Students = finalSheet->class2_students();
    FinalSheet::StudentData studentData = class2Students[row];

    if( col >= 2 && studentData.sub_homework.size()-col+2 >= 0 && col < table_homeWork2->columnCount()-1){
        studentData.sub_homework[col-2] = QVariant(item->text().toInt());
        qDebug()<<"changed item : "<<studentData.sub_homework[col-2];
    }
    //计算值变化后的平均成绩
    double mean_total = 0.0;
    for(const auto& a: studentData.sub_homework){
        mean_total += a.toDouble();
    }
    mean_total = mean_total/studentData.sub_homework.size();

    int res_mean = static_cast<int>(std::round(mean_total));
    studentData.homework = QVariant(res_mean);
    QStandardItem* t_item_hom = table_homeWork2->item(row,studentData.sub_homework.size()+2);
    t_item_hom->setText(studentData.homework.toString());

    class2Students[row] = studentData;
    finalSheet->setClass2Students(class2Students);
    //刷新model2
    operExcel->countTotalScore();
    ui->tableView->viewport()->update();
    QStandardItem* t_item_toal = table_model2->item(row,3);
    t_item_toal->setText(finalSheet->class2_students().at(row).homework.toString());
    // 连接信号和槽
    connect(table_homeWork2, &QStandardItemModel::itemChanged, this,
            &MainWindow::handleItemChanged_homeworkView2);
}

void MainWindow::showMessageBox(const QString &message)
{//消息框
    QMessageBox msgBox;
    msgBox.setStyleSheet("QLabel { color: red; font-weight: bold; letter-spacing: 2px; text-align: center; }");
    msgBox.setText(message);
    msgBox.exec();
}

/**
 * @brief MainWindow::save_Iniconfig
 * @param ret
 * 1.保存 每个 model 上的 item && 每个ui->tableview 上的 model
 * 2.保存一班 二班学生的 信息
 * 3.保存课程信息
 * 4.保存控件 信息
 *
 */
void MainWindow::save_Iniconfig(bool &ret)
{//保存配置文件的实现
    if(finalSheet == nullptr) ret = false;


    finalSheet->setclass1Config();
    finalSheet->setclass2Config();
    finalSheet->setCourseDataConfig();

    if(MySettings::instance().getIshided()){
        MySettings::instance().hideFile();
    }
    ret = true;


}
/**
 * @brief MainWindow::save_Iniconfig
 * //保存配置文件的实现 定时保存(暂定)
 */
void MainWindow::save_Iniconfig()
{

}

void MainWindow::read_Iniconfig(bool &ret)
{
    if(finalSheet == nullptr) ret = false;
    finalSheet->readclass1FromConfig();
    finalSheet->readclass2FromConfig();

    //考勤信息初始化
    if(table_attdendance == nullptr)
    {
        table_attdendance = new QStandardItemModel;
        QStringList heardLabels;
        heardLabels<<"学号1"<<"姓名1"<<"1考勤次数"<<"学号2"<<"姓名2"<<"2考勤次数";
        table_attdendance->setHorizontalHeaderLabels(heardLabels);
        operExcel->setAttdendanceViewModel(table_attdendance);
        connect(table_attdendance,&QStandardItemModel::itemChanged,this,
                &MainWindow::handleItemChanged_attendance);
    }

    QStringList heardLaber;
    heardLaber<<"学号"<<"姓名";
    if(table_experiment1 == nullptr){
        table_experiment1 = new QStandardItemModel(this);
        table_experiment1->setHorizontalHeaderLabels(heardLaber);

    }
    if(table_experiment2 == nullptr){
        table_experiment2 = new QStandardItemModel(this);
        table_experiment2->setHorizontalHeaderLabels(heardLaber);
    }

    if(table_homeWork1 == nullptr){
        table_homeWork1 = new QStandardItemModel(this);
        table_homeWork1->setHorizontalHeaderLabels(heardLaber);
    }
    if(table_homeWork2 == nullptr){
        table_homeWork2 = new QStandardItemModel(this);
        table_homeWork2->setHorizontalHeaderLabels(heardLaber);
    }
    operExcel->setHomeWorkViewModel(table_homeWork1,1);
    operExcel->setHomeWorkViewModel(table_homeWork2,2);

    operExcel->setClassTableViewModel(table_model1,1);
    operExcel->setClassTableViewModel(table_model2,2);

    operExcel->setExperimentViewModel(table_experiment1,1);
    operExcel->setExperimentViewModel(table_experiment2,2);


    ui->tableView_3->setModel(table_attdendance);
    ui->tableView->setModel(table_model1);
    ui->tableView_2->setModel(table_model2);

    finalSheet->readCourseDataConfig();

    ret = true;
}

//# end MainWindow.cpp
void MainWindow::on_ac_homework_triggered()
{//显示作业成绩 && 平时成绩
    if(path == "NullPath" &&  notConfig)
    {
        showMessageBox("请先导入文件");
        return;
    }
    if(ui->tableView->model() == table_homeWork1){return;}
    if(ui->tableView_2->model() == table_homeWork2){return;}

    ui->tableView->setModel(table_homeWork1);
    ui->tableView_2->setModel(table_homeWork2);

    // 首先断开之前的连接
    disconnect(table_homeWork1, &QStandardItemModel::itemChanged, this,
               &MainWindow::handleItemChanged_homeworkView1);
    disconnect(table_homeWork2, &QStandardItemModel::itemChanged, this,
               &MainWindow::handleItemChanged_homeworkView2);

    connect(table_homeWork1,&QStandardItemModel::itemChanged,this,
            &MainWindow::handleItemChanged_homeworkView1);
    connect(table_homeWork2,&QStandardItemModel::itemChanged,this,
            &MainWindow::handleItemChanged_homeworkView2);

}

// 实验成绩
void MainWindow::on_ac_experimentScore_triggered()
{
    if(path == "NullPath" &&  notConfig)
    {
        showMessageBox("请先导入文件");
        return;
    }

    if(ui->tableView->model() == table_experiment1){return;}
    if(ui->tableView_2->model() == table_experiment2){return;}
    ui->tableView->setModel(table_experiment1);
    ui->tableView_2->setModel(table_experiment2);

    // 首先断开之前的连接
    disconnect(table_experiment1, &QStandardItemModel::itemChanged, this,
               &MainWindow::handleItemChanged_experimentView1);
    disconnect(table_experiment2, &QStandardItemModel::itemChanged, this,
               &MainWindow::handleItemChanged_experimentView2);

    connect(table_experiment1,&QStandardItemModel::itemChanged,this,
            &MainWindow::handleItemChanged_experimentView1);
    connect(table_experiment2,&QStandardItemModel::itemChanged,this,
            &MainWindow::handleItemChanged_experimentView2);

}

//跳转回成绩汇总表
void MainWindow::on_ac_toallSocre_triggered()
{
    if(path == "NullPath" &&  notConfig)
    {
        showMessageBox("请先导入文件");
        return;
    }
    ui->tableView->setModel(table_model1);
    ui->tableView_2->setModel(table_model2);

}

void MainWindow::on_ac_final_overall_triggered()
{
    if(path == "NullPath" &&  notConfig)
    {
        showMessageBox("请先导入文件");
        return;
    }
}


void MainWindow::on_ac_v_Info_triggered()
{//版本信息
    AboutDialog aboutDialog(this); // 创建 AboutDialog 的实例
    aboutDialog.exec(); // 显示对话框
}

void MainWindow::on_ac_saveSettings_triggered()
{//保存配置文件
    bool ret;
    save_Iniconfig(ret);
    if(ret){
        qDebug()<<"保存成功!";
    }else{
        qDebug()<<"保存失败!";
    }
}

/**
 * @brief MainWindow::on_ac_loadSettings_triggered
 *读取配置文件
 */
void MainWindow::on_ac_loadSettings_triggered()
{
    bool ret = false;
    read_Iniconfig(ret);
    if(ret){
        qDebug()<<"读取成功!";
        this->notConfig = false;
    }else{
        qDebug()<<"读取失败!";
    }
}

void MainWindow::on_ac_hidden_configFile_triggered(bool checked)
{
    if(checked){
        ui->ac_hidden_configFile->setText("取消隐藏配置文件");
        MySettings::instance().setIshided(true);
        MySettings::instance().hideFile();
    }else{
        ui->ac_hidden_configFile->setText("隐藏配置文件");
        MySettings::instance().setIshided(false);
        MySettings::instance().unhideFile();
    }
}

