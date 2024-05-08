#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

//  1. toolBar 添加 考勤 作业 实验 actions
//  2. 系统级目录 存放 保存配置 加载配置
//  3. 焦点功能打开 单击 修改内容
//  4. 添加模板功能 （读取信息的模板）既原表
//  5. 修改默认学年直接读

}

