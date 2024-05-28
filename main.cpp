#include "mainwindow.h"
#include <QApplication>
#include <QPalette>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.resize(803,600);
    w.setStyleSheet(QString("QMenuBar {"
                            "    background-color: #333333;"  // 菜单栏背景色
                            "    color: #FFFFFF;"  // 菜单栏文字颜色
                            "    font-weight: bold;"
                            "}"
                            "QMenu {"
                            "    background-color: #444444;"  // 菜单项背景色
                            "    color: #FFD700;"  // 菜单项文字颜色
                            "    border: 1px solid #555555;"
                            "}"
                            "QMenu::item:selected {"
                            "    background-color: #555555;"  // 选中的菜单项背景色
                            "    color: #FFD700;"  // 选中的菜单项文字颜色
                            "}"
                            "QTableView {"
                            "    selection-background-color: #00AEEF;"  // 选中项的背景颜色
                            "    gridline-color: #808000;"  // 网格线颜色
                            "    alternate-background-color: #6A5ACD;"  // 交替行背景色
                            "    background-color: #FFFFFF;" // 表格背景色
                            "    color: #000000;" // 文字颜色
                            "}"
                            "QHeaderView::section {"
                            "    background-color: #CCCCCC;"  // 表头背景色
                            "    color: #333333;"  // 表头文字颜色
                            "    padding: 4px;"
                            "    border: 1px solid #DDDDDD;"
                            "}"
                            "QToolBar {"
                            "    background-color: #2F4F4F;"  // 工具栏背景色
                            "    border: 1px solid #1C1C1C;"
                            "}"
                            "QToolButton {"
                            "    background-color: #2F4F4F;"  // 工具按钮背景色
                            "    color: #FFFFFF;"  // 工具按钮文字颜色
                            "    border: none;"
                            "    padding: 5px;"
                            "}"
                            "QToolButton:hover {"
                            "    background-color: #4F4F4F;"  // 工具按钮悬停背景色
                            "}"
                            "QToolButton:checked {"
                            "    background-color: #FFD700;"  // 工具按钮选中背景色
                            "    color: #333333;"  // 工具按钮选中文字颜色
                            "    border: 1px solid #FFA500;"
                            "}"
                            "QStatusBar {"
                            "    background-color: #2F4F4F;"  // 状态栏背景色
                            "    color: #FFFFFF;"  // 状态栏文字颜色
                            "    border-top: 1px solid #FF4500;"
                            "}"
                            "QStatusBar QLabel {"
                            "    color: #FFFFFF;"  // Label文字颜色
                            "}"
                            ));

    w.show();
    return a.exec();
}

