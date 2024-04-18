#include "operexcel.h"

QXLSX_USE_NAMESPACE            // 添加Xlsx命名空间

#define EXCEL_NAME "./1.xlsx"  // 本demo中用到的excel文件路径文件名

OperExcel::OperExcel()
{

}

void OperExcel::creat_New_Excel(QString path, bool &ret)
{
    Document xlsx;
    ret = xlsx.saveAs(path);
}
