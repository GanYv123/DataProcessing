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

// (若可以 添加选项卡 设置班级)

//bug1 控件 存在重复点击在激活状态下被激活的问题
//bug2 模板文件 行在复制时字体大小属性继承错误 成绩字号 为 11

    //最大的  bug当修改的表格中没有对应的源数据时 会导致越界访问 程序崩溃!!!
}

