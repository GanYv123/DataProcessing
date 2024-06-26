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
#include "sqldata.h"
#include "QStyleOptionSlider"
#include "QHeaderView"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),finalSheet(new FinalSheet(this))
    , mainwidget_finalScore(new FinalScoreMainWidget(this))
{
    ui->setupUi(this);

    this->setAcceptDrops(true);
    ui->tableView->setAcceptDrops(false);
    this->customDialog = new CustomDialog(this);
    this->customDialog->setAcceptDrops(false);
    customDialog->close();
    operExcel = new OperExcel(this,finalSheet);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);

    timerSQL = new QTimer(this);
    connect(timerSQL, &QTimer::timeout, this, &MainWindow::onTimerSQLTimeout);

    initMainWindow();
    connect(this,&MainWindow::student_added,this,&MainWindow::slots_student_added);
    connect(table_model1,&QStandardItemModel::itemChanged,this,&MainWindow::handleItemChanged1);
    connect(table_model2,&QStandardItemModel::itemChanged,this,&MainWindow::handleItemChanged2);

    connect(this,&MainWindow::addStuSQLSuccessful,this,[this](){showMessageBox("添加成功!");});
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
    // 初始化 QActionGroup
    this->actionGroup = new QActionGroup(this);
    this->actionGroup->setExclusive(true); // 设置互斥
    this->actionGroup->addAction(ui->ac_homework);
    this->actionGroup->addAction(ui->ac_experimentScore);
    this->actionGroup->addAction(ui->ac_toallSocre);
    this->actionGroup->addAction(ui->ac_final_overall);
    ui->ac_toallSocre->setChecked(true);

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

    if(MySettings::instance().getIshided()){
        ui->ac_hidden_configFile->setText("取消隐藏配置");
        QIcon icon(":/images2/visibility_24dp_FILL0_wght400_GRAD0_opsz24.png");
        ui->ac_hidden_configFile->setIcon(icon);
    }else{

        ui->ac_hidden_configFile->setText("隐藏配置文件");
        QIcon icon(":/images2/visibility_off_24dp.png");
        ui->ac_hidden_configFile->setIcon(icon);
    }

    //信号连接
    setActionsContextMenu(); //右键菜单
    setTableViewHeaderConnect(); //双击

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
    if(isopen){
        showMessageBox("已打开 文件/配置\n若需要加载新文件请重启软件");return;
    }
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
        isopen = true;
        //考勤信息初始化
        update_dataview();

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


/**
 * @brief MainWindow::on_ac_creatFiles_triggered
 * 新建文件，当点击时跳转 新建文件的Widget,在新建文件的 widget 里面创建课程信息到 finalsheet
 */
void MainWindow::on_ac_creatFiles_triggered() //demo function BETA 1
{

    dialog_newFileDialog = new NewFileDialog(this);
    connect(dialog_newFileDialog,&NewFileDialog::sendCourseData,this,&MainWindow::getCourseDataForSignal);
    dialog_newFileDialog->show();
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
// void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
// {
//     //禁用函数
//     Q_UNUSED(index);
//     return;
// }


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
 * @todo 待修改函数 修改插入位置 （默认为插入到头部）
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

        changedDataStack.append({t_stu,1}); //记录更新

        stu_1.insert(0,t_stu); //插入位置 后期修改为可选择插入位置
        finalSheet->setClass1Students(stu_1);
    }

    if(list.last()->toInt() == 2) {
        qDebug() << "2班学生" << *list.at(0) << " " << *list.at(1);
        QVector <FinalSheet::StudentData> stu_2 = finalSheet->class2_students();
        FinalSheet::StudentData t_stu;
        t_stu.studentName = QVariant(*list.at(0));
        t_stu.studentID = QVariant(*list.at(1));
        t_stu.sub_experiment.resize(stu_2.at(0).sub_experiment.size());
        t_stu.sub_homework.resize(stu_2.at(0).sub_homework.size());

        changedDataStack.append({t_stu,2});

        stu_2.insert(0,t_stu);
        finalSheet->setClass2Students(stu_2);
    }

    // 3. 禁用信号和槽的连接以防止多余的更新
    table_model1->blockSignals(true);
    table_experiment1->blockSignals(true);
    table_homeWork1->blockSignals(true);
    table_model2->blockSignals(true);
    table_experiment2->blockSignals(true);
    table_homeWork2->blockSignals(true);
    table_attdendance->blockSignals(true);

    update_dataview();

    // 5. 恢复信号和槽的连接
    table_model1->blockSignals(false);
    table_experiment1->blockSignals(false);
    table_homeWork1->blockSignals(false);
    table_model2->blockSignals(false);
    table_experiment2->blockSignals(false);
    table_homeWork2->blockSignals(false);
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

void MainWindow::onTimerTimeout()
{//定时器溢出函数实现自动保存
    bool ret = false;
    save_Iniconfig(ret);
    if(!ret){
        showMessageBox("自动保存失败");
    }else{
        // 设置标签文本为成功自动保存配置 + 当前系统时间
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString currentDateTimeString = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
        this->label_tips->setText("成功自动保存配置 " + currentDateTimeString);
    }
}

//自动更新数据库
void MainWindow::onTimerSQLTimeout()
{
    if(!SQLData::instance().isLinked()){
        qDebug()<<"未连接数据库";
        return;
    }
    // while(!changedDataStack.empty()){
    //     //执行更新语句
    //     QPair<FinalSheet::StudentData,int> pair = changedDataStack.pop();
    //     SQLData::instance().updateSQl(pair.first,pair.second);
    // }

    QString sql = "TRUNCATE TABLE students";

    if(SQLData::instance().executeUpdate(sql))
        emit on_ac_update_triggered();

    qDebug()<<"SQL 数据库 已更新！";
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

    changedDataStack.append({studentData,1}); //记录变化

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

    changedDataStack.append({studentData,2}); //记录变化

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
    msgBox.setStyleSheet("QLabel { color: red; font-weight: bold; letter-spacing: 2px; text-align: center; }"
                         "QPushButton::pressed{"
                         "       background-color: #444444;"
                         "       border:0;"
                         "       color:red;"
                         "}"
                         );
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
    MySettings::instance().saveTimerData("chooseTime",selectedOption);

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

//读取配置
void MainWindow::read_Iniconfig(bool &ret)
{

    if(finalSheet == nullptr || !MySettings::instance().configExists()){ ret = false;return;}
    finalSheet->readclass1FromConfig();
    finalSheet->readclass2FromConfig();


    finalSheet->readCourseDataConfig();

    update_dataview();

    MySettings::instance().loadTimerData("chooseTime",selectedOption);
    if (selectedOption == "不启动自动保存") {
        // 处理不启动自动保存的逻辑
        QMessageBox::information(this, "选择的选项", "关闭自动保存");
        // 停止定时器
        timer->stop();
    } else {
        // 处理选择的时间间隔
        QMessageBox::information(this, "选择的选项", "您选择的自动保存时间间隔是: " + selectedOption);
        // 启动定时器
        if (selectedOption == "30秒") {
            timer->start(30 * 1000); // 30秒，单位为毫秒
        } else if (selectedOption == "1分钟") {
            timer->start(60 * 1000); // 1分钟，单位为毫秒
        } else if (selectedOption == "5分钟") {
            timer->start(5 * 60 * 1000); // 5分钟，单位为毫秒
        } else if (selectedOption == "10分钟") {
            timer->start(10 * 60 * 1000); // 10分钟，单位为毫秒
        } else if (selectedOption == "15分钟") {
            timer->start(15 * 60 * 1000); // 15分钟，单位为毫秒
        } else if (selectedOption == "30分钟") {
            timer->start(30 * 60 * 1000); // 30分钟，单位为毫秒
        } else if (selectedOption == "1小时") {
            timer->start(60 * 60 * 1000); // 1小时，单位为毫秒
        } else {
            // 默认情况下，启动定时器，并且使用默认间隔（例如 5 分钟）
            timer->start(5 * 60 * 1000);
        }
    }

    ret = true;
}

/**
 * @brief MainWindow::update_dataview
 * @warning 更新视图
 */
void MainWindow::update_dataview()
{
    //考勤信息初始化
    if(table_attdendance == nullptr)
    {
        table_attdendance = new QStandardItemModel;
    }

    operExcel->setAttdendanceViewModel(table_attdendance);
    connect(table_attdendance,&QStandardItemModel::itemChanged,this,
            &MainWindow::handleItemChanged_attendance);

    QStringList heardLaber,_heardLaber;
    heardLaber<<"学号"<<"姓名";
    _heardLaber = heardLaber;
    try {
        // 检查 finalSheet 是否为空指针
        if (!finalSheet) {
            throw std::runtime_error("finalSheet is a null pointer.");
        }

        // 检查 class1_students 容器是否为空或索引是否超出范围
        const auto& students = finalSheet->class1_students();
        if (students.size() <= 1) {
            throw std::out_of_range("class1_students container size is too small.");
        }

        // 检查 sub_experiment 容器是否为空
        const auto& subExperiments = students.at(0).sub_experiment;
        if (subExperiments.isEmpty()) {
            throw std::out_of_range("sub_experiment container is empty.");
        }

        // 遍历 sub_experiment 容器
        for (qsizetype i = 1; i <= subExperiments.size(); ++i) {
            heardLaber.append(QString("实验%1").arg(i));
        }
        for (qsizetype i = 1; i <= students.at(0).sub_homework.size(); ++i) {
            _heardLaber.append(QString("作业%1").arg(i));
        }

    } catch (const std::out_of_range &e) {
        qDebug() << "std::out_of_range caught:" << e.what();
        // 处理索引越界异常
    } catch (const std::runtime_error &e) {
        qDebug() << "std::runtime_error caught:" << e.what();
        // 处理空指针异常
    } catch (const std::exception &e) {
        qDebug() << "Standard exception caught:" << e.what();
        // 处理其他标准异常
    } catch (...) {
        qDebug() << "Unknown exception caught.";
        // 处理其他未知异常
    }

    if(table_experiment1 == nullptr){
        table_experiment1 = new QStandardItemModel(this);
    }
    table_experiment1->setHorizontalHeaderLabels(heardLaber);

    if(table_experiment2 == nullptr){
        table_experiment2 = new QStandardItemModel(this);
    }
    table_experiment2->setHorizontalHeaderLabels(heardLaber);

    //-----------------homework------------------------//
    if(table_homeWork1 == nullptr){
        table_homeWork1 = new QStandardItemModel(this);
    }
    table_homeWork1->setHorizontalHeaderLabels(_heardLaber);
    if(table_homeWork2 == nullptr){
        table_homeWork2 = new QStandardItemModel(this);
    }

    table_homeWork2->setHorizontalHeaderLabels(_heardLaber);

    operExcel->setHomeWorkViewModel(table_homeWork1,1);
    operExcel->setHomeWorkViewModel(table_homeWork2,2);

    operExcel->setClassTableViewModel(table_model1,1);
    operExcel->setClassTableViewModel(table_model2,2);

    operExcel->setExperimentViewModel(table_experiment1,1);
    operExcel->setExperimentViewModel(table_experiment2,2);

    QStringList heardLabels;
    heardLabels<<"学号"<<"姓名"<<QString("考勤%%1").arg(finalSheet->getCourseData().rate_attendance.toString())
                <<QString("作业%%1").arg(finalSheet->getCourseData().rate_homework.toString())
                <<QString("实验%%1").arg(finalSheet->getCourseData().rate_experiment.toString())
                <<"总成绩"<<"备注";

    table_model1->setHorizontalHeaderLabels(heardLabels);
    table_model2->setHorizontalHeaderLabels(heardLabels);

    QStringList heardLabels1;
    heardLabels1<<"学号1"<<"姓名1"<<"1考勤次数"<<"学号2"<<"姓名2"<<"2考勤次数";
    table_attdendance->setHorizontalHeaderLabels(heardLabels1);

    ui->tableView_3->setModel(table_attdendance);
    ui->tableView->setModel(table_model1);
    ui->tableView_2->setModel(table_model2);
}

void MainWindow::sortByID(bool &ret)
{
    // 1. 获取并排序班级1的学生数据
    QVector<FinalSheet::StudentData> stu_1 = finalSheet->class1_students();
    std::sort(stu_1.begin(), stu_1.end(), [](const FinalSheet::StudentData &a, const FinalSheet::StudentData &b) {
        QString idA = a.studentID.toString();
        QString idB = b.studentID.toString();
        QString lastTwoDigitsA = idA.right(2);  // 获取学号的后两位
        QString lastTwoDigitsB = idB.right(2);  // 获取学号的后两位
        return lastTwoDigitsA.toInt() < lastTwoDigitsB.toInt();
    });
    finalSheet->setClass1Students(stu_1);

    // 2. 获取并排序班级2的学生数据
    QVector<FinalSheet::StudentData> stu_2 = finalSheet->class2_students();
    std::sort(stu_2.begin(), stu_2.end(), [](const FinalSheet::StudentData &a, const FinalSheet::StudentData &b) {
        QString idA = a.studentID.toString();
        QString idB = b.studentID.toString();
        QString lastTwoDigitsA = idA.right(2);  // 获取学号的后两位
        QString lastTwoDigitsB = idB.right(2);  // 获取学号的后两位
        return lastTwoDigitsA.toInt() < lastTwoDigitsB.toInt();
    });
    finalSheet->setClass2Students(stu_2);

    // 3. 禁用信号和槽的连接以防止多余的更新
    table_model1->blockSignals(true);
    table_experiment1->blockSignals(true);
    table_homeWork1->blockSignals(true);
    table_model2->blockSignals(true);
    table_experiment2->blockSignals(true);
    table_homeWork2->blockSignals(true);
    table_attdendance->blockSignals(true);

    update_dataview();

    // 5. 恢复信号和槽的连接
    table_model1->blockSignals(false);
    table_experiment1->blockSignals(false);
    table_homeWork1->blockSignals(false);
    table_model2->blockSignals(false);
    table_experiment2->blockSignals(false);
    table_homeWork2->blockSignals(false);
    table_attdendance->blockSignals(false);

    ret = true;
}

void MainWindow::sortByTotalScore(bool &ret)
{
    // 1. 获取并排序班级1的学生数据
    QVector<FinalSheet::StudentData> stu_1 = finalSheet->class1_students();
    std::sort(stu_1.begin(), stu_1.end(), [](const FinalSheet::StudentData &a, const FinalSheet::StudentData &b) {
        return a.totalScore.toDouble() > b.totalScore.toDouble();  // 按总成绩从高到低排序
    });
    finalSheet->setClass1Students(stu_1);

    // 2. 获取并排序班级2的学生数据
    QVector<FinalSheet::StudentData> stu_2 = finalSheet->class2_students();
    std::sort(stu_2.begin(), stu_2.end(), [](const FinalSheet::StudentData &a, const FinalSheet::StudentData &b) {
        return a.totalScore.toDouble() > b.totalScore.toDouble();  // 按总成绩从高到低排序
    });
    finalSheet->setClass2Students(stu_2);

    // 3. 禁用信号和槽的连接以防止多余的更新
    table_model1->blockSignals(true);
    table_experiment1->blockSignals(true);
    table_homeWork1->blockSignals(true);
    table_model2->blockSignals(true);
    table_experiment2->blockSignals(true);
    table_homeWork2->blockSignals(true);
    table_attdendance->blockSignals(true);

    update_dataview();

    // 5. 恢复信号和槽的连接
    table_model1->blockSignals(false);
    table_experiment1->blockSignals(false);
    table_homeWork1->blockSignals(false);
    table_model2->blockSignals(false);
    table_experiment2->blockSignals(false);
    table_homeWork2->blockSignals(false);
    table_attdendance->blockSignals(false);

    ret = true;  // 设置返回值为 true，表示排序成功
}

/**
 * @brief MainWindow::setActionsContextMenu
 * 设置 右键菜单 右键菜单 选择设置添加行数和删除行数
 */
void MainWindow::setActionsContextMenu()
{
    QHeaderView *horizontalHeader1 = ui->tableView->horizontalHeader();
    QHeaderView *horizontalHeader2 = ui->tableView_2->horizontalHeader();

    horizontalHeader1->setContextMenuPolicy(Qt::CustomContextMenu);
    horizontalHeader2->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(horizontalHeader1,&QHeaderView::customContextMenuRequested,
            this,&MainWindow::showHeaderContextMenu);

    connect(horizontalHeader2,&QHeaderView::customContextMenuRequested,
            this,&MainWindow::showHeaderContextMenu);
}

/**
 * @brief MainWindow::setTableViewHeaderConnect
 * 用于 tableview 的 single & slot 的 双击 连接
 */
void MainWindow::setTableViewHeaderConnect()
{
    connect(ui->tableView->horizontalHeader(),&QHeaderView::sectionDoubleClicked,
            this,&MainWindow::onHeaderDoubleClicked_tableview);

    connect(ui->tableView_2->horizontalHeader(),&QHeaderView::sectionDoubleClicked,
            this,&MainWindow::onHeaderDoubleClicked_tableview_2);
}

/**
 * @brief MainWindow::showHeaderContextMenu
 * @param pos 参数表示在表头视图中的本地坐标（即相对于表头视图的坐标），
 * 这用于确定右键点击事件发生的位置。
 */
void MainWindow::showHeaderContextMenu(const QPoint &pos)
{
    QHeaderView *header = qobject_cast<QHeaderView*>(sender());
    if (!header)
        return;

    if(ui->tableView->model() == table_model1 ||
        ui->tableView->model() == table_model2){
        return;
    }

    // 获取被点击的表头索引
    int logicalIndex = header->logicalIndexAt(pos);
    int columnCount = header->model()->columnCount();

    // 检查索引是否是第一个、第二个或最后一个表头
    if (logicalIndex == 0 || logicalIndex == 1 || logicalIndex == columnCount - 1) {
        return; // 如果是第一个、第二个或最后一个表头，不显示菜单
    }

    // 创建右键菜单
    QMenu contextMenu;
    contextMenu.addAction(ui->ac_addcol);
    contextMenu.addAction(ui->ac_drop_col);

    // 显示菜单
    contextMenu.exec(header->viewport()->mapToGlobal(pos));
}

/**
 * @brief MainWindow::onHeaderDoubleClicked
 * 更具当前视图上的模型的不同实现 不同的项 双击之后的跳转
 * @title 双击跳转函数
 *
 */
void MainWindow::onHeaderDoubleClicked_tableview(int logicalIndex)
{
    QAbstractItemModel* model = ui->tableView->model();
    if(model == table_model1){
        qDebug()<<"tableview_1触发跳转 "<<logicalIndex;
        switch (logicalIndex) {
        case 2:
            ui->tabWidget->setCurrentIndex(2);
            ui->ac_toallSocre->setChecked(false);
            break;
        case 3:
            ui->tableView->setModel(table_homeWork1);
            ui->ac_homework->trigger();
            break;
        case 4:
            ui->tableView->setModel(table_experiment1);
            ui->ac_experimentScore->trigger();
            break;
        default:
            break;
        }
    }
}

void MainWindow::onHeaderDoubleClicked_tableview_2(int logicalIndex)
{
    QAbstractItemModel* model = ui->tableView_2->model();
    if(model == table_model2){
        qDebug()<<"tableview_2触发跳转 "<<logicalIndex;
    }
}

void MainWindow::on_ac_homework_triggered()
{//显示作业成绩 && 平时成绩

    ui->tabWidget->setCurrentIndex(0);

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
    ui->tabWidget->setCurrentIndex(0);

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
    ui->tabWidget->setCurrentIndex(0);
    ui->tableView->setModel(table_model1);
    ui->tableView_2->setModel(table_model2);

}

/**
 * @brief MainWindow::on_ac_final_overall_triggered
 * 跳转 期末 详情 表
 */
void MainWindow::on_ac_final_overall_triggered()
{
    mainwidget_finalScore->show();
    connect(mainwidget_finalScore,&FinalScoreMainWidget::destroyedWithSignal,this,&MainWindow::show);
    this->hide();
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
        this->label_tips->setText("保存成功!");
    }else{
        qDebug()<<"保存失败!";
        this->label_tips->setText("保存失败!");
    }
}

/**
 * @brief MainWindow::on_ac_loadSettings_triggered
 *读取配置文件
 */
void MainWindow::on_ac_loadSettings_triggered()
{
    if(isopen){
        showMessageBox("已打开 文件/配置\n若需要加载新文件请重启软件");return;
    }
    bool ret = false;
    read_Iniconfig(ret);
    if(ret){
        qDebug()<<"读取成功!";
        this->label_tips->setText("读取配置文件成功!");
        this->setWindowTitle("已导入配置文件");
        this->isopen = true;
        this->notConfig = false;
    }else{
        qDebug()<<"读取失败!";
        this->label_tips->setText("读取配置文件失败!");
         showMessageBox("读取配置文件失败!");
    }
}

void MainWindow::on_ac_hidden_configFile_triggered(bool checked)
{
    if(!MySettings::instance().configExists()){
        showMessageBox("未查询到配置文件!");
        ui->ac_hidden_configFile->setChecked(!checked);
        return;
    }
    if(checked){
        ui->ac_hidden_configFile->setText("取消隐藏配置");
        QIcon icon(":/images2/visibility_24dp_FILL0_wght400_GRAD0_opsz24.png");
        ui->ac_hidden_configFile->setIcon(icon);
        MySettings::instance().setIshided(true);
        MySettings::instance().hideFile();
        this->label_tips->setText("取消隐藏配置文件");
    }else{
        ui->ac_hidden_configFile->setText("隐藏配置文件");
        QIcon icon(":/images2/visibility_off_24dp.png");
        ui->ac_hidden_configFile->setIcon(icon);
        MySettings::instance().setIshided(false);
        MySettings::instance().unhideFile();
        this->label_tips->setText("隐藏配置文件");
    }
}


void MainWindow::on_ac_setConfigPath_triggered()
{//设置配置文件保存位置 默认为 软件运行的目录
    MySettings::instance().selectConfigFilePath();
    showMessageBox(QString("配置文件路径为:%1\n下次需要导入配置文件请重新定位该目录")
                       .arg(MySettings::instance().getCONFIG_FILE_NAME()));
}


void MainWindow::on_ac_export_template_triggered()
{//导出模板
    QString dir = QFileDialog::getExistingDirectory(this,
                "选择目录", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        QString fileName = QFileDialog::getSaveFileName(this, "保存模板",
                                                        dir + "/MOBAN.xlsx", "Excel 文件 (*.xlsx)");
        if (!fileName.isEmpty()) {
            QFile file(":/excel_formwork/MOBAN.xlsx");
            if (!file.copy(fileName)) {
                QMessageBox::warning(this, "错误", "无法复制模板文件。");
            } else {
                QMessageBox::information(this, "成功", "模板文件导出成功。");
            }
        } else {
            QMessageBox::warning(this, "警告", "未提供文件名。");
        }
    } else {
        QMessageBox::warning(this, "警告", "未选择目录。");
    }

}

void MainWindow::deleteStudent(int classNumber, int rowIndex) {

    if (classNumber == 1) {
        QVector<FinalSheet::StudentData> stu_1 = finalSheet->class1_students();
        if (rowIndex >= 0 && rowIndex < stu_1.size()) {
            stu_1.remove(rowIndex);
            finalSheet->setClass1Students(stu_1);

        } else {
            qDebug() << "Invalid row index for class 1.";
        }
    } else if (classNumber == 2) {
        QVector<FinalSheet::StudentData> stu_2 = finalSheet->class2_students();
        if (rowIndex >= 0 && rowIndex < stu_2.size()) {
            stu_2.remove(rowIndex);
            finalSheet->setClass2Students(stu_2);

            // 暂时禁用信号和槽的连接
            table_model2->blockSignals(true);
            table_experiment2->blockSignals(true);
            table_homeWork2->blockSignals(true);

        }
    } else {
        qDebug() << "Invalid class number.";
    }
    // 3. 禁用信号和槽的连接以防止多余的更新
    table_model1->blockSignals(true);
    table_experiment1->blockSignals(true);
    table_homeWork1->blockSignals(true);
    table_model2->blockSignals(true);
    table_experiment2->blockSignals(true);
    table_homeWork2->blockSignals(true);
    table_attdendance->blockSignals(true);

    update_dataview();

    // 5. 恢复信号和槽的连接
    table_model1->blockSignals(false);
    table_experiment1->blockSignals(false);
    table_homeWork1->blockSignals(false);
    table_model2->blockSignals(false);
    table_experiment2->blockSignals(false);
    table_homeWork2->blockSignals(false);
    table_attdendance->blockSignals(false);
}

/**
 * @brief MainWindow::on_ac_deleteStu_triggered
 * 学生 删除按钮 点击
 */
void MainWindow::on_ac_deleteStu_triggered() {
    if (this->path == "NullPath" && this->notConfig) {
        qWarning() << "未导入文件|配置";
        this->label_tips->setText("未导入文件|");
        showMessageBox("未导入文件|配置");
        return;
    }

    DeleteStudentDialog dialog(this);

    connect(&dialog, &DeleteStudentDialog::classSelectionChanged, this, [&dialog, this](int classNumber) {
        dialog.clearStudentList();
        if (classNumber == 1) {
            QVector<FinalSheet::StudentData> stu_1 = finalSheet->class1_students();
            for (const auto &stu : stu_1) {
                dialog.addStudentToList(stu.studentID.toString(), stu.studentName.toString(), 1);
            }
        } else if (classNumber == 2) {
            QVector<FinalSheet::StudentData> stu_2 = finalSheet->class2_students();
            for (const auto &stu : stu_2) {
                dialog.addStudentToList(stu.studentID.toString(), stu.studentName.toString(), 2);
            }
        }
    });

    emit dialog.classSelectionChanged(1);

    if (dialog.exec() == QDialog::Accepted) {
        int classNumber = dialog.getSelectedClass();
        int rowIndex = dialog.getSelectedRow();

        if (rowIndex >= 0) {
            deleteStudent(classNumber, rowIndex);
        } else {
            qWarning() << "没有选中任何学生";
        }
    }
}

void MainWindow::on_ac_template_triggered()
{//模板规范信息
    QUrl url("https://github.com/GanYv123/DataProcessing/blob/master/README.md");
    QDesktopServices::openUrl(url);

}


void MainWindow::on_ac_sortByID_triggered()
{//点击的话排序学生 按照学号
    if (this->path == "NullPath" && this->notConfig) {
        qWarning() << "未导入文件|配置";
        this->label_tips->setText("未导入文件|配置");
        showMessageBox("未导入文件|配置");
        return;
    }
    bool ret = false;
    sortByID(ret);
    if(ret){
        QMessageBox::information(this,"tips","排序成功");
    }else{
        QMessageBox::warning(this,"error!","未知的错误");
    }
}


void MainWindow::on_ac_sortByToalScore_triggered()
{//按照平时分排序
    if (this->path == "NullPath" && this->notConfig) {
        qWarning() << "未导入文件|配置";
        this->label_tips->setText("未导入文件|配置");
        showMessageBox("未导入文件|配置");
        return;
    }
    bool ret = false;
    sortByTotalScore(ret);
    if(ret){
        QMessageBox::information(this,"tips","排序成功");
    }else{
        QMessageBox::warning(this,"error!","未知的错误");
    }
}


void MainWindow::on_ac_autoConfigTime_triggered()
{//设置自动保存配置时间
    // 创建一个对话框
    QDialog dialog(this);
    dialog.setWindowTitle("设置自动保存配置时间");
    // 创建布局
    QVBoxLayout layout;
    // 创建QComboBox控件
    QComboBox *comboBox = new QComboBox(&dialog);
    comboBox->addItem("不启动自动保存");
    comboBox->addItem("30秒");
    comboBox->addItem("1分钟");
    comboBox->addItem("5分钟");
    comboBox->addItem("10分钟");
    comboBox->addItem("15分钟");
    comboBox->addItem("30分钟");
    comboBox->addItem("1小时");

    // 创建确定和取消按钮
    QPushButton *okButton = new QPushButton("确定", &dialog);
    QPushButton *cancelButton = new QPushButton("取消", &dialog);

    // 将控件添加到布局中
    layout.addWidget(comboBox);
    layout.addWidget(okButton);
    layout.addWidget(cancelButton);

    // 设置对话框布局
    dialog.setLayout(&layout);

    // 连接确定和取消按钮的信号槽
    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    // 显示对话框并获取用户输入
    if (dialog.exec() == QDialog::Accepted) {
        selectedOption = comboBox->currentText();
        if (selectedOption == "不启动自动保存") {
            // 处理不启动自动保存的逻辑
            QMessageBox::information(this, "选择的选项", "关闭自动保存");
            // 停止定时器
            timer->stop();
        } else {
            // 处理选择的时间间隔
            QMessageBox::information(this, "选择的选项", "您选择的自动保存时间间隔是: " + selectedOption);
            // 启动定时器
            if (selectedOption == "30秒") {
                timer->start(30 * 1000); // 30秒，单位为毫秒
            } else if (selectedOption == "1分钟") {
                timer->start(60 * 1000); // 1分钟，单位为毫秒
            } else if (selectedOption == "5分钟") {
                timer->start(5 * 60 * 1000); // 5分钟，单位为毫秒
            } else if (selectedOption == "10分钟") {
                timer->start(10 * 60 * 1000); // 10分钟，单位为毫秒
            } else if (selectedOption == "15分钟") {
                timer->start(15 * 60 * 1000); // 15分钟，单位为毫秒
            } else if (selectedOption == "30分钟") {
                timer->start(30 * 60 * 1000); // 30分钟，单位为毫秒
            } else if (selectedOption == "1小时") {
                timer->start(60 * 60 * 1000); // 1小时，单位为毫秒
            } else {
                // 默认情况下，启动定时器，并且使用默认间隔（例如 5 分钟）
                timer->start(5 * 60 * 1000);
            }
        }
    }
}

void MainWindow::on_ac_linkDataBase_triggered()
{//连接数据库
    connectSQLDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QString host = dialog.hostLineEdit->text();
        int port = dialog.portLineEdit->text().toInt();
        QString user = dialog.userLineEdit->text();
        QString password = dialog.passwordLineEdit->text();
        QString dbName = dialog.dbNameLineEdit->text();

        // 连接数据库
        if (SQLData::instance().connect(host, port, user, password, dbName)) {
            qDebug() << "Successfully connected to the database";
            showMessageBox("Successfully connected to the database");
        } else {
            qDebug() << "Failed to connect to the database";
            showMessageBox("Failed to connect to the database");
        }
    }
}

void MainWindow::on_ac_unLink_triggered()
{
    if(SQLData::instance().isLinked()){
        SQLData::instance().disconnect();
        showMessageBox("断开成功!");
    }else{
        showMessageBox("数据库未连接，无需断开!");
    }
}

/**
 * @brief MainWindow::on_ac_update_triggered
 * 暂且测试为 将 class1 & class2 学生信息写道 数据库 单独开一个线程运行
 */
void MainWindow::on_ac_update_triggered()
{//写入更新数据
    if(!SQLData::instance().isLinked()){
        showMessageBox("请先连接数据库");
        return;
    }
    std::thread threadAddStu(&MainWindow::threadFunctionAddStudentsToSQL, this);
    threadAddStu.detach();
}


void MainWindow::on_ac_download_triggered()
{
    if(!this->notConfig){
        QMessageBox::StandardButton box;
        box = QMessageBox::question(this, "提示", "覆盖原有的数据?", QMessageBox::Yes|QMessageBox::No);
        //取消
        if(box==QMessageBox::No)
            return;
        //确认
        else if(box==QMessageBox::Yes)
        {
            //读取数据库中的数据
            if(!SQLData::instance().isLinked()){
                showMessageBox("数据库未连接!");
                return;
            }

            QMap <int,QVector<FinalSheet::StudentData>>studata_map(SQLData::instance().readStudentData());
            finalSheet->setClass1Students(studata_map.value(1));
            finalSheet->setClass2Students(studata_map.value(2));
            finalSheet->setCourseData(SQLData::instance().readCourseData());

            update_dataview();
            this->notConfig = false;
        }
        return;
    }
    //读取数据库中的数据
    if(!SQLData::instance().isLinked()){
        showMessageBox("数据库未连接!");
        return;
    }

    QMap <int,QVector<FinalSheet::StudentData>>studata_map(SQLData::instance().readStudentData());
    finalSheet->setClass1Students(studata_map.value(1));
    finalSheet->setClass2Students(studata_map.value(2));
    finalSheet->setCourseData(SQLData::instance().readCourseData());

    update_dataview();
    this->notConfig = false;

}

/**
 * @brief MainWindow::threadFunctionAddStudentsToSQL
 *  子线程，创建表并且添加学生信息
 */
void MainWindow::threadFunctionAddStudentsToSQL()
{
    //检测表 是否存在 不存在则创建
    SQLData::instance().CreateStudentsTable();
    SQLData::instance().CreateCourseTable();

    if(!finalSheet->class1_students().empty()){
        for(const auto& stu1 : finalSheet->class1_students()){
            if(!SQLData::instance().insertStudentData(stu1,1)){
                emit addStuSQLFailed();
            }
        }
    }
    if(!finalSheet->class2_students().empty()){
        for(const auto& stu2 : finalSheet->class2_students()){
            if(!SQLData::instance().insertStudentData(stu2,2)){
                emit addStuSQLFailed();
            }
        }
    }
    if(!SQLData::instance().insertCourseData(finalSheet->getCourseData())){
        emit addStuSQLFailed();
    }
    emit addStuSQLSuccessful();
}

/**
 * @brief MainWindow::on_ac_deleteTable_triggered
 * debug 后面 要删除的 函数
 */
void MainWindow::on_ac_deleteTable_triggered()
{
    QString deletetable = QString("DROP TABLE %1,%2").arg("students").arg("CourseData");
    if(SQLData::instance().executeUpdate(deletetable)){
        showMessageBox("successful");
    }else{
        showMessageBox("error");
    }
}

void MainWindow::on_ac_autoConfigTime_2_triggered()
{
    // 创建一个对话框
    QDialog dialog(this);
    dialog.setWindowTitle("设置SQL自动保存配置时间");
    // 创建布局
    QVBoxLayout layout;
    // 创建QComboBox控件
    QComboBox *comboBox = new QComboBox(&dialog);
    comboBox->addItem("不启动自动保存");
    comboBox->addItem("30秒");
    comboBox->addItem("1分钟");
    comboBox->addItem("5分钟");
    comboBox->addItem("10分钟");
    comboBox->addItem("15分钟");
    comboBox->addItem("30分钟");
    comboBox->addItem("1小时");

    // 创建确定和取消按钮
    QPushButton *okButton = new QPushButton("确定", &dialog);
    QPushButton *cancelButton = new QPushButton("取消", &dialog);

    // 将控件添加到布局中
    layout.addWidget(comboBox);
    layout.addWidget(okButton);
    layout.addWidget(cancelButton);

    // 设置对话框布局
    dialog.setLayout(&layout);

    // 连接确定和取消按钮的信号槽
    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    // 显示对话框并获取用户输入
    if (dialog.exec() == QDialog::Accepted) {
        selectedOption = comboBox->currentText();
        if (selectedOption == "不启动自动保存") {
            // 处理不启动自动保存的逻辑
            QMessageBox::information(this, "选择的选项", "关闭自动保存");
            // 停止定时器
            timerSQL->stop();
        } else {
            // 处理选择的时间间隔
            QMessageBox::information(this, "选择的选项", "您选择的自动保存时间间隔是: " + selectedOption);
            // 启动定时器
            if (selectedOption == "30秒") {
                timerSQL->start(30 * 1000); // 30秒，单位为毫秒
            } else if (selectedOption == "1分钟") {
                timerSQL->start(60 * 1000); // 1分钟，单位为毫秒
            } else if (selectedOption == "5分钟") {
                timerSQL->start(5 * 60 * 1000); // 5分钟，单位为毫秒
            } else if (selectedOption == "10分钟") {
                timerSQL->start(10 * 60 * 1000); // 10分钟，单位为毫秒
            } else if (selectedOption == "15分钟") {
                timerSQL->start(15 * 60 * 1000); // 15分钟，单位为毫秒
            } else if (selectedOption == "30分钟") {
                timerSQL->start(30 * 60 * 1000); // 30分钟，单位为毫秒
            } else if (selectedOption == "1小时") {
                timerSQL->start(60 * 60 * 1000); // 1小时，单位为毫秒
            } else {
                // 默认情况下，启动定时器，并且使用默认间隔（例如 5 分钟）
                timerSQL->start(5 * 60 * 1000);
            }
        }
    }
}


void MainWindow::on_ac_setWindowOpacity_triggered()
{
    // 创建一个 QDialog 作为滑块控件的容器
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Set Window Opacity");

    // 创建滑块控件
    CustomSlider cslider;
    QSlider *slider = cslider.createSliderWithStyle(dialog);

    // 创建标签显示当前值
    QLabel *label = new QLabel(QString::number(this->windowOpacity() * 100) + "%", dialog);

    // 连接滑块值的变化信号到槽函数
    connect(slider, &QSlider::valueChanged, this, [=](int value){
        double opacity = value / 100.0;
        this->setWindowOpacity(opacity);
        label->setText(QString::number(value) + "%");
    });

    // 设置布局
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(slider);
    layout->addWidget(label);
    dialog->setLayout(layout);

    // 显示对话框
    dialog->exec();
}

/**
 * @brief MainWindow::getCourseDataForSignal
 * 得到 新建文件ui传来的 模板的 信息
 * @param courseData
 */
void MainWindow::getCourseDataForSignal(const FinalSheet::CourseData courseData) const
{
    finalSheet->setCourseData(courseData);
    qDebug()<<"模板设置成功!";
    this->label_tips->setText("设置课程信息成功！");
}
