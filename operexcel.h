#ifndef OPEREXCEL_H
#define OPEREXCEL_H
//这个类用来操作excel 导入qxlsl库
#include "xlsxdocument.h"
#include "QObject"
#include "QVariantMap"
#include "finalsheet.h"

class QStandardItemModel;
class FinalSheet;
class MainWindow;

class OperExcel
{

protected:
    void fillData(QXlsx::Document& xlsx);
    void oper_data_class1(QXlsx::Document& xlsx);
    void oper_data_class2(QXlsx::Document& xlsx);

public:
    OperExcel();
    OperExcel(MainWindow*parent_mainWindow,FinalSheet* finalSheet = nullptr);
    void creat_New_Excel(QString &path,bool& ret); //新建表格
    void open_Excel(QString &path,bool& ret,QObject *parent);//打开文件
    QStandardItemModel* getQStandardItemModelPoint();
    void save_Excel(QString &path,bool& ret,QObject *parent = nullptr);
    void export_Excel(QString &path,bool& ret,QObject *parent = nullptr);

    QVariantMap* get_course_information();
    void read_course_information();
    void setViewModel(QStandardItemModel* o_model);


private:
    MainWindow *m_parent_mainWindow = nullptr;

    QXlsx::Document *m_xlsx = nullptr;

    QStandardItemModel *m_model = nullptr;

    QVariantMap *course_information = nullptr;

    FinalSheet *m_finalSheet = nullptr;
};

#endif // OPEREXCEL_H
