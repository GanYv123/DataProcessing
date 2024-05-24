#include "mainwindow.h"
#include <QApplication>
#include <QPalette>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.setStyleSheet(QString("QMenuBar {"
                            "    background-color: #f9cbff;"  // 菜单栏背景色
                            "    color: #2a2626;"  // 菜单栏文字颜色
                            "}"
                            "QMenu {"
                            "    background-color: #9efd8a;"  // 菜单项背景色
                            "    color: #000000;"  // 菜单项文字颜色
                            "}"
                            "QMenu::item:selected {"
                            "    background-color: #afc8f8;"  // 选中的菜单项背景色
                            "    color: #571001;"  // 选中的菜单项文字颜色
                            "}"
                            "QTableView {"
                            "    selection-background-color: #ff9f84;"  // 选中项的背景颜色
                            "}"));
    w.show();
    return a.exec();
}

