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
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
    ui->tableView->resize(this->width()-40,this->height()-50);
    this->setMinimumSize(500,400);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 初始状态下禁止编辑

    //初始化tableView
    if(table_model1 == nullptr)
        table_model1 = new QStandardItemModel(this);
    if(table_model2 == nullptr)
        table_model2 = new QStandardItemModel(this);

    QStringList heardLabels;
    heardLabels<<"班级"<<"学号"<<"姓名"<<"考勤"<<"实验"<<"作业"<<"总成绩"<<"备注";

    table_model1->setHorizontalHeaderLabels(heardLabels);
    table_model2->setHorizontalHeaderLabels(heardLabels);



    //需要有优化逻辑
    ui->tableView->setModel(table_model1);
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
    ui->tableView->resize(sz.width()-40,sz.height()-50);
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
        //文件打开成功就返回加到好数据的模型
        //table_model = operExcel->getQStandardItemModelPoint();
        //ui->tableView->setModel(table_model);
    }else{
        this->label_tips->setText("文件未打开");
        qWarning()<<"文件未打开!";
    }

}

void MainWindow::setLabel_CourseInfo(const QString &text)
{
    this->label_CourseInfo->setText(text);
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
    // 检查索引的有效性
    if (index.isValid()) {
        ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked
                                       | QAbstractItemView::SelectedClicked);  // 双击时允许编辑
        // 获取选定单元格的数据
        QVariant currentValue = index.data(Qt::DisplayRole);

        // 弹出对话框或其他方式让用户编辑数据
        bool ok;
        QString newValue = QInputDialog::getText(this, tr("Edit Data"),
                                                 tr("Enter new value:"), QLineEdit::Normal,
                                                 currentValue.toString(), &ok);
        QVariant newValue_copy = QVariant(newValue);

        // 如果用户点击了 OK 按钮并且输入了新值，则更新数据模型中的数据
        if (ok && !newValue_copy.isNull()) {
            // 获取模型对象
            QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->tableView->model());
            if (model) {
                // 修改数据模型中的数据
                model->setData(index, newValue_copy, Qt::EditRole);

                // 刷新视图，以显示更新后的数据
                ui->tableView->viewport()->update();
            }
        }
    }
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
    bool ret;
    this->operExcel->export_Excel(path,ret,this);
    if(ret){
        qDebug()<<"导出成功";
    }else{
        qDebug()<<"导出失败";
    }
}


void MainWindow::on_ac_Save_as_triggered()
{//文件另存为

}

/**
@test 待修改函数
@title 演示函数
*/
void MainWindow::on_ac_addStu_triggered()
{//添加学生
    QVector<FinalSheet::StudentData> studentData = finalSheet->getStudentData("通信工程21-1");

    FinalSheet::StudentData t_studentData;

    QVariant t_classID = QVariant("通信工程21-1");
    QVariant t_studentID = QVariant("2021050301xx");
    QVariant t_studentName =QVariant("姓名");
    QVariant t_attendance = QVariant(0.00);
    QVariant t_homework = QVariant(0.00);
    QVariant t_experiment = QVariant(0.00);

    t_studentData.studentID = t_studentID;
    t_studentData.attendance = t_attendance;
    t_studentData.studentName = t_studentName;
    t_studentData.homework = t_homework;
    t_studentData.experiment = t_experiment;

    studentData.push_back(t_studentData);



    QList<QStandardItem*> itemList;
    itemList.append(new QStandardItem(t_classID.toString()));
    itemList.append(new QStandardItem(t_studentData.studentID.toString()));
    itemList.append(new QStandardItem(t_studentData.studentName.toString()));
    itemList.append(new QStandardItem(QString::number(t_studentData.attendance.toDouble())));
    itemList.append(new QStandardItem(QString::number(t_studentData.homework.toDouble())));
    itemList.append(new QStandardItem(QString::number(t_studentData.experiment.toDouble())));

    finalSheet->setStudentData(studentData); //从修改完成的模型添加
    emit student_added(itemList);
}

void MainWindow::slots_student_added(QList<QStandardItem*> itemList)
{//添加完学生更新视图

    //后期添加 选择班级的判断


    table_model1->appendRow(itemList);
    ui->tableView->setModel(table_model1);
    ui->tableView->resizeColumnsToContents();
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if (index.isValid()) {
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 单击时禁止编辑
    }
}


void MainWindow::on_ac_checkMajor_triggered()
{//点击选择的班级 切换 不同班级的 model

    this->customDialog_chooseClassID = new CustomDialog_chooseClassID("txgc 21-1 ","txgc 21-2 ",this);
    customDialog_chooseClassID->move(this->width()/2-150,this->height()/2-100);
    customDialog_chooseClassID->resize(300,200);
    customDialog_chooseClassID->show_chooseClassID();
    //根据选择班级切换模型
}

