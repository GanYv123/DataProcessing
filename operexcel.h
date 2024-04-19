#ifndef OPEREXCEL_H
#define OPEREXCEL_H
//这个类用来操作excel 导入qxlsl库
#include "xlsxdocument.h"
#include "QObject"

class QStandardItemModel;

class OperExcel
{
public:
    OperExcel();
    void creat_New_Excel(QString &path,bool& ret); //新建表格
    void open_Excel(QString &path,bool& ret,QObject *parent);//打开文件
    QStandardItemModel* getQStandardItemModelPoint();
    void save_Excel(QString &path,bool& ret,QObject *parent = nullptr);


private:
    QXlsx::Document *m_xlsx = nullptr;

    QStandardItemModel *model = nullptr;
};

#endif // OPEREXCEL_H
