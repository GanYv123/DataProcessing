#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "file_operations.h"
#include "QList"
#include "customdialog.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include "QActionGroup"


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

protected:
    void showMessageBox(const QString &message);
    void save_Iniconfig(bool &ret);
    void save_Iniconfig();

    void read_Iniconfig(bool &ret);

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

    QStandardItemModel* getClass1Model();
    QStandardItemModel* getClass2Model();

    void deleteStudent(int classNumber, int rowIndex);


signals:
    void student_added(QList<QVariant*> list);

private slots:
    void on_ac_openFiles_triggered();

    void on_ac_creatFiles_triggered();

    void on_ac_choose_school_year_triggered();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_ac_saveFiles_triggered();

    void on_ac_exportExcel_triggered();

    void on_ac_addStu_triggered();

    void slots_student_added(QList<QVariant*> list);

    void on_tableView_clicked(const QModelIndex &index);

    void on_ac_checkMajor_triggered();

    void on_ac_Attendance_triggered();

    //模型项改变
    void handleItemChanged1(QStandardItem *item);
    void handleItemChanged2(QStandardItem *item);
    void handleItemChanged_attendance(QStandardItem *item);
    void handleItemChanged_experimentView1(QStandardItem *item);
    void handleItemChanged_experimentView2(QStandardItem *item);
    void handleItemChanged_homeworkView1(QStandardItem *item);
    void handleItemChanged_homeworkView2(QStandardItem *item);

    void on_ac_homework_triggered();

    void on_ac_experimentScore_triggered();

    void on_ac_toallSocre_triggered();

    void on_ac_final_overall_triggered();

    void on_ac_v_Info_triggered();

    void on_ac_saveSettings_triggered();

    void on_ac_loadSettings_triggered();

    void on_ac_hidden_configFile_triggered(bool checked);

    void on_ac_setConfigPath_triggered();

    void on_ac_export_template_triggered();

    void on_ac_deleteStu_triggered();

public:
    CustomDialog *customDialog = nullptr;

    //CustomDialog_chooseClassID *customDialog_chooseClassID = nullptr;

    qint8 currentChooseClassID = -1;

private:
    Ui::MainWindow *ui;

    OperExcel *operExcel = nullptr;
    QStandardItemModel *table_model1 = nullptr; //班级 1
    QStandardItemModel *table_model2 = nullptr; //班级 2
    QStandardItemModel *table_attdendance = nullptr; //考勤表

    QStandardItemModel *table_homeWork1 = nullptr; //作业成绩表
    QStandardItemModel *table_experiment1 = nullptr; //实验成绩表
    QStandardItemModel *table_homeWork2 = nullptr; //作业成绩表
    QStandardItemModel *table_experiment2 = nullptr; //实验成绩表

    QStandardItemModel *table_final_overall1 = nullptr; //总评成绩
    QStandardItemModel *table_final_overall2 = nullptr;

    QString path = "NullPath";
    bool notConfig = true;
    //最终表单
    FinalSheet *finalSheet = nullptr;
    QActionGroup *actionGroup;

    bool isopen = false; //判断是否打开文件

};
#endif // MAINWINDOW_H
