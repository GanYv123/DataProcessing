#include "mainwindow.h"
#include <QApplication>
#include <QPalette>
#include <QSharedMemory>
#include <QMessageBox>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QSharedMemory sharedMemory;
    sharedMemory.setKey("Hi3ts2u3yo1");

    MainWindow w;


    // 尝试创建共享内存段
    if (sharedMemory.attach()) {
        // 如果能attach到共享内存段，说明已有实例在运行
        QMessageBox::warning(&w,"error","已经打开进程");
        return 0;
    }

    if (!sharedMemory.create(1)) {
        // 如果创建共享内存段失败，也说明已有实例在运行
        QMessageBox::warning(&w,"error","已经打开进程");
        return 0;
    }
    w.resize(832,600);
    w.setStyleSheet(QString("QToolBar {"
                            "    background-color: #F8F8F8;"   // 工具栏背景色
                            "    border: 1px solid #E7E7E7;"   // 工具栏边框色
                            "    padding: 5px;"                // 工具栏内边距
                            "}"
                            "QToolButton {"
                            "    background-color: #FFFFFF;"   // 工具按钮背景色
                            "    color: #007AFF;"              // 工具按钮文字颜色
                            "    border: 1px solid #C7C7CC;"   // 工具按钮边框色
                            "    border-radius: 10px;"         // 工具按钮圆角
                            "    padding: 5px;"                // 工具按钮内边距
                            "}"
                            "QToolButton:hover {"
                            "    background-color: #E5E5E5;"   // 工具按钮悬停背景色
                            "}"
                            "QToolButton:checked {"
                            "    background-color: #007AFF;"   // 工具按钮选中背景色
                            "    color: #FFFFFF;"              // 工具按钮选中文字颜色
                            "}"
                            "QTableView {"
                            "    selection-background-color: #778899;"   // 表格选中项背景色
                            "    gridline-color: #B0C4DE;"               // 表格网格线颜色
                            "    alternate-background-color: #F8F8F8;"  // 表格交替行背景色
                            "    background-color: #FFFFFF;"             // 表格背景色
                            "    color: #000000;"                        // 表格文字颜色
                            "}"
                            "QHeaderView::section {"
                            "    background-color: #F8F8F8;"   // 表头背景色
                            "    color: #000000;"              // 表头文字颜色
                            "    padding: 5px;"                // 表头内边距
                            "    border: 1px solid #E7E7E7;"   // 表头边框颜色
                            "}"
                            "QMenuBar {"
                            "    background-color: #F8F8F8;"   // 菜单栏背景色
                            "    color: #000000;"              // 菜单栏文字颜色
                            "    font-weight: bold;"           // 菜单栏文字加粗
                            "    border-bottom: 1px solid #E7E7E7;"   // 菜单栏底部边框
                            "}"
                            "QMenuBar::item:selected {"
                            "    background-color: #E5E5E5;"   // 选中菜单项背景色
                            "}"
                            "QStatusBar {"
                            "    background-color: #F8F8F8;"   // 状态栏背景色
                            "    color: #000000;"              // 状态栏文字颜色
                            "    border-top: 1px solid #E7E7E7;"   // 状态栏顶部边框
                            "}"
                            ));


    w.show();
    return a.exec();
}

