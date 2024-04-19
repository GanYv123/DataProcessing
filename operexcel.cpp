#include "operexcel.h"

QXLSX_USE_NAMESPACE            // 添加Xlsx命名空间

#define EXCEL_NAME "./1.xlsx"  // 本demo中用到的excel文件路径文件名

OperExcel::OperExcel()
{

}

void textDemoUnit1(Document& x){
    x.write(1,1,"学期起");
    x.write(1,2,"学期末");
    x.write(1,3,"学期");
    x.write(1,4,"班级1");
    x.write(1,5,"人数1");
    x.write(1,6,"班级2");
    x.write(1,7,"人数2");

    x.write(2,1,2023);
    x.write(2,2,2024);
    x.write(2,3,1);
    x.write(2,4,"物联网21-1"); x.setColumnWidth(4,15);
    x.write(2,5,42);
    x.write(2,6,"物联网21-2"); x.setColumnWidth(6,15);
    x.write(2,7,40);


}

void OperExcel::creat_New_Excel(QString path, bool &ret)
{
    Document xlsx;
    textDemoUnit1(xlsx);
    ret = xlsx.saveAs(path);   
}

void OperExcel::open_Excel(QString path, bool &ret)
{//打开文件
    if(!m_xlsx)
    {
        m_xlsx = new Document(path);
    }
    ret = m_xlsx->load();


}
