#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QLabel"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,f_oper(new File_Operations)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainWindow();
    f_oper->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMainWindow()
{//初始化界面组件
    QWidget *label = new QLabel("状态栏pass",this);
    ui->statusbar->addWidget(label);

}

void MainWindow::on_ac_openFiles_triggered() //打开文件
{

}

