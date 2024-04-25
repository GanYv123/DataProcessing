#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "file_operations.h"
#include "QList"
#include "customdialog.h"
#include <QStandardItemModel>
#include <QStandardItem>


//声明类
class QLabel;
class QUrl;
class OperExcel;
class FinalSheet;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QLabel *label_size,*label_tips;
    QUrl filePath; //打开文件的地址
    QLabel *label_CourseInfo;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initMainWindow();

    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    void handleFile(const QString &filePath);
    void setLabel_CourseInfo(const QString &text);


signals:
    void student_added(QList<QStandardItem*>);

private slots:
    void on_ac_openFiles_triggered();

    void on_ac_creatFiles_triggered();

    void on_ac_choose_school_year_triggered();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_ac_saveFiles_triggered();

    void on_ac_exportExcel_triggered();

    void on_ac_Save_as_triggered();

    void on_ac_addStu_triggered();

    void slots_student_added(QList<QStandardItem*> itemList);

    void on_tableView_clicked(const QModelIndex &index);

    void on_ac_checkMajor_triggered();

public:
    CustomDialog *customDialog = nullptr;
    CustomDialog_chooseClassID *customDialog_chooseClassID = nullptr;

private:
    Ui::MainWindow *ui;

    OperExcel *operExcel = nullptr;
    QStandardItemModel *table_model1 = nullptr; //班级 1
    QStandardItemModel *table_model2 = nullptr; //班级 2
    QString currentChooseClassID = "";

    QString path;
    //最终表单
    FinalSheet *finalSheet = nullptr;

};
#endif // MAINWINDOW_H
