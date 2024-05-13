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

MainWindow::~MainWindow()
{
    if(ui)
        delete ui;
}
/**
 * @brief MainWindow::initMainWindow 初始化界面组件
 */
void MainWindow::initMainWindow()
{//初始化界面组件
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
            connect(table_attdendance,&QStandardItemModel::itemChanged,this,&MainWindow::handleItemChanged_attendance);
        }
        ui->tableView_3->setModel(table_attdendance);

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
{//新建文件
    if(!operExcel)
        operExcel = new OperExcel();
    bool ret;
    QString path = "./1.xlsx";
    operExcel->creat_New_Excel(path,ret); //修改未 可自己选择文件位置的 函数
    if(ret){
        qInfo()<<"创建成功";
    }else{
        qWarning()<<"创建失败!";
    }

}

void MainWindow::on_ac_choose_school_year_triggered()
{//下拉框选择学年
    if(customDialog == nullptr)
        this->customDialog = new CustomDialog(this);
    customDialog->move(this->width()/2-150,this->height()/2-100);
    customDialog->resize(300,200);
    customDialog->show_choose_schoolYears();

}

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
    if(this->operExcel == nullptr || this->path == "NullPath")
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
{//添加学生
    if(this->operExcel == nullptr || this->path == "NullPath")
    {
        qWarning()<<"未导入文件";
        this->label_tips->setText("未导入文件");
        return;
    }
    QVector<FinalSheet::StudentData> studentData = finalSheet->getStudentData("");

    FinalSheet::StudentData t_studentData;

    QVariant t_studentID = QVariant("2021050301xx");
    QVariant t_studentName =QVariant("姓名");
    QVariant t_attendanceScore = QVariant(0);
    QVariant t_homework = QVariant(0.00);
    QVariant t_experiment = QVariant(0.00);

    t_studentData.studentID = t_studentID;
    t_studentData.attendanceScore = t_attendanceScore;
    t_studentData.studentName = t_studentName;
    t_studentData.homework = t_homework;
    t_studentData.experiment = t_experiment;

    studentData.push_back(t_studentData);

    QList<QStandardItem*> itemList;
    itemList.append(new QStandardItem(t_studentData.studentID.toString()));
    itemList.append(new QStandardItem(t_studentData.studentName.toString()));
    itemList.append(new QStandardItem(QString::number(t_studentData.attendanceScore.toDouble())));
    itemList.append(new QStandardItem(QString::number(t_studentData.homework.toDouble())));
    itemList.append(new QStandardItem(QString::number(t_studentData.experiment.toDouble())));

    finalSheet->setStudentData(studentData); //从修改完成的模型添加
    emit student_added(itemList);
}

/**
 * @brief MainWindow::slots_student_added
 * @param itemList
 * @todo 待修改函数
 */
void MainWindow::slots_student_added(QList<QStandardItem*> itemList)
{//添加完学生更新视图

    //后期添加 选择班级的判断
    if(this->currentChooseClassID == 1){
        table_model1->appendRow(itemList);
        ui->tableView->setModel(table_model1);
        //ui->tableView->resizeColumnsToContents();
    }else if(this->currentChooseClassID == 2){
        table_model2->appendRow(itemList);
        ui->tableView->setModel(table_model2);
        //ui->tableView->resizeColumnsToContents();
    }else{
        qDebug()<<"没有选择正确的班级";
    }
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
    if(path == "NullPath")
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

    QVariant total = ( (attdance.toDouble() * courseData.rate_attendance.toDouble()/100.0) +
                      (homework.toDouble() * courseData.rate_homework.toDouble()/100.0) +
                      (experiment.toDouble() * courseData.rate_experiment.toDouble()/100.0));

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

    QVariant total = ( (attdance.toDouble() * courseData.rate_attendance.toDouble()/100.0) +
                      (homework.toDouble() * courseData.rate_homework.toDouble()/100.0) +
                      (experiment.toDouble() * courseData.rate_experiment.toDouble()/100.0));

    QStandardItem* t_item = table_model2->item(row,5);
    t_item->setText(total.toString());

    class2Students[row] = studentData;

    // 通过 FinalSheet 更新数据
    finalSheet->setClass2Students(class2Students);
    //刷新model2
    operExcel->countTotalScore();
    ui->tableView->viewport()->update();
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

    if(col == class1Index){
        // 获取对应的数据项
        QVector<FinalSheet::StudentData> class1Students = finalSheet->class1_students();
        FinalSheet::StudentData studentData = class1Students[row];

        studentData.attendance = QVariant(item->text());
        class1Students[row] = studentData;

        //同步修改到 finallySheet
        int attScore = (finalSheet->getCourseData().attendance_reduce_fractions.toInt()*
                        (finalSheet->getCourseData().lessonTime.toInt()/2 -
                         class1Students.at(row).attendance.toInt()));
        class1Students[row].attendanceScore = 100-attScore;

        finalSheet->setClass1Students(class1Students);
        qDebug()<<"修改 后的次数 "<<finalSheet->class1_students()[row].attendance.toString()<<
            "final score = "<<finalSheet->class1_students()[row].attendanceScore.toString();
        //修改 model1的data
        this->table_model1->item(row,2)->setText(
            finalSheet->class1_students().at(row).attendanceScore.toString());

        operExcel->countTotalScore();
        ui->tableView->viewport()->update();

    }else if (col == class2Index){
        QVector<FinalSheet::StudentData> class2Students = finalSheet->class2_students();
        FinalSheet::StudentData studentData = class2Students[row];

        studentData.attendance = QVariant(item->text());
        class2Students[row] = studentData;

        //同步修改到 finallySheet
        int attScore = (finalSheet->getCourseData().attendance_reduce_fractions.toInt()*
                        (finalSheet->getCourseData().lessonTime.toInt()/2 -
                         class2Students.at(row).attendance.toInt()));
        class2Students[row].attendanceScore = 100-attScore;

        finalSheet->setClass2Students(class2Students);
        qDebug()<<"修改 后的次数 "<<finalSheet->class2_students()[row].attendance.toString()<<
            "final score = "<<finalSheet->class2_students()[row].attendanceScore.toString();
        //修改 model1的data
        this->table_model2->item(row,2)->setText(
            finalSheet->class2_students().at(row).attendanceScore.toString());

        operExcel->countTotalScore();
        ui->tableView->viewport()->update();
    }
}


void MainWindow::showMessageBox(const QString &message)
{//消息框
    QMessageBox msgBox;
    msgBox.setStyleSheet("QLabel { color: red; font-weight: bold; letter-spacing: 2px; text-align: center; }");
    msgBox.setText(message);
    msgBox.exec();
}

//# end MainWindow.cpp

void MainWindow::on_ac_homework_triggered()
{//显示作业成绩 && 平时成绩
    if(path == "NullPath")
    {
        showMessageBox("请先导入文件");
        return;
    }

}

void MainWindow::on_ac_experimentScore_triggered()
{// 实验成绩
    if(path == "NullPath")
    {
        showMessageBox("请先导入文件");
        return;
    }

    if(ui->tableView->model() == table_experiment1){return;}
    if(ui->tableView_2->model() == table_experiment2){return;}

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

    //设置模型项
    operExcel->setExperimentViewModel(table_experiment1,1);
    operExcel->setExperimentViewModel(table_experiment2,2);

    ui->tableView->setModel(table_experiment1);
    ui->tableView_2->setModel(table_experiment2);
}

void MainWindow::on_ac_toallSocre_triggered()
{//跳转回成绩汇总表
    if(path == "NullPath")
    {
        showMessageBox("请先导入文件");
        return;
    }
    ui->tableView->setModel(table_model1);
    ui->tableView_2->setModel(table_model2);

}

