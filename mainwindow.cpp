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
#include "QInputDialog"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainWindow();
    this->setAcceptDrops(true);
    ui->tableView->setAcceptDrops(false);
    this->customDialog = new CustomDialog(this);
    this->customDialog->setAcceptDrops(false);
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
    ui->statusbar->addWidget(new QWidget(),1);
    ui->statusbar->addPermanentWidget(label_size);

    ui->tableView->move(20,20);
    ui->tableView->resize(this->width()-40,this->height()-50);
    this->setMinimumSize(500,400);
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
    // 在这里实现对文件的具体处理逻辑
    if(!operExcel)
        operExcel = new OperExcel();

    path = filePath;
    //path.replace("\\","/");
    qDebug() << "Handling file: " << path;

    bool ret = false;
    operExcel->open_Excel(path,ret,this);
    if(ret){
        this->label_tips->setText("文件打开成功!");
        qInfo()<<"文件打开成功！";
        //文件打开成功就返回加到好数据的模型
        table_model = operExcel->getQStandardItemModelPoint();
        ui->tableView->setModel(table_model);
    }else{
        this->label_tips->setText("文件未打开");
        qWarning()<<"文件未打开!";
    }

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
        // 获取选定的行和列
        int row = index.row();
        int col = index.column();

        // 获取选定单元格的数据
        QVariant currentValue = index.data(Qt::DisplayRole);

        // 弹出对话框或其他方式让用户编辑数据
        bool ok;
        QString newValue = QInputDialog::getText(this, tr("Edit Data"),
                                                 tr("Enter new value:"), QLineEdit::Normal,
                                                 currentValue.toString(), &ok);

        // 如果用户点击了 OK 按钮并且输入了新值，则更新数据模型中的数据
        if (ok && !newValue.isEmpty()) {
            // 获取模型对象
            QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->tableView->model());
            if (model) {
                // 修改数据模型中的数据
                model->setData(index, newValue, Qt::EditRole);

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

