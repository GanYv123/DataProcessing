#ifndef OPEREXCEL_H
#define OPEREXCEL_H
//这个类用来操作excel 导入qxlsl库
#include "xlsxdocument.h"

class OperExcel
{
public:
    OperExcel();
    void creat_New_Excel(QString path,bool& ret);

private:
    QXlsx::Document* m_xlsx = nullptr;
};

#endif // OPEREXCEL_H
