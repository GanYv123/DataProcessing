#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "file_operations.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initMainWindow();

private slots:
    void on_ac_openFiles_triggered();

private:
    Ui::MainWindow *ui;
    File_Operations* f_oper;
};
#endif // MAINWINDOW_H
