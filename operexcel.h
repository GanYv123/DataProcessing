#ifndef OPEREXCEL_H
#define OPEREXCEL_H
//这个类用来操作excel 导入qxlsl库
#include "xlsxdocument.h"
#include "QObject"
#include "QVariantMap"

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
    OperExcel(MainWindow*parent_mainWindow);
    void creat_New_Excel(QString &path,bool& ret); //新建表格
    void open_Excel(QString &path,bool& ret,QObject *parent);//打开文件
    QStandardItemModel* getQStandardItemModelPoint();
    void save_Excel(QString &path,bool& ret,QObject *parent = nullptr);
    void export_Excel(QString &path,bool& ret,QObject *parent = nullptr);

    QVariantMap* get_course_information();
    void read_course_information();


private:
    MainWindow *m_parent_mainWindow = nullptr;

    QXlsx::Document *m_xlsx = nullptr;

    QStandardItemModel *model = nullptr;

    QVariantMap *course_information = nullptr;

    FinalSheet *finalSheet;
};

#endif // OPEREXCEL_H
