#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QLabel"
#include "QFile"
#include "QSize"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainWindow();
    this->setAcceptDrops(true);

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
    ui->tableView->setAcceptDrops(false);
    ui->tableView->move(20,20);
    ui->tableView->resize(this->width()-40,this->height()-50);
    this->setMinimumSize(500,400);

}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) //拖拽进入
{

}

void MainWindow::dragMoveEvent(QDragMoveEvent *event) //进入移动时
{

}

void MainWindow::dropEvent(QDropEvent *event) //放入时
{

}

void MainWindow::resizeEvent(QResizeEvent *event) //尺寸改变
{
    QSize sz = this->size();
    ui->tableView->resize(sz.width()-40,sz.height()-50);
    this->label_size->setText(QString("size: %1 , %2").arg(sz.width()).arg(sz.height()));
    event->accept();//事件接收
}

void MainWindow::on_ac_openFiles_triggered() //打开文件
{

}

