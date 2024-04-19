#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "file_operations.h"
#include "customdialog.h"

//声明类
class QLabel;
class QUrl;
class OperExcel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QLabel *label_size,*label_tips;
    QUrl filePath; //打开文件的地址

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initMainWindow();

    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    void handleFile(const QString &filePath);

private slots:
    void on_ac_openFiles_triggered();

    void on_ac_creatFiles_triggered();

    void on_ac_choose_school_year_triggered();

private:
    Ui::MainWindow *ui;

    OperExcel*operExcel = nullptr;
    CustomDialog*customDialog = nullptr;
};
#endif // MAINWINDOW_H
