#include "operexcel.h"
#include "QStandardItem"
#include "QStandardItemModel"
#include "QVariant"

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

void OperExcel::creat_New_Excel(QString &path, bool &ret)
{
    Document xlsx;
    textDemoUnit1(xlsx);
    ret = xlsx.saveAs(path);
}

void OperExcel::open_Excel(QString &path, bool &ret,QObject *parent)
{//打开文件
    if(m_xlsx == nullptr){
        m_xlsx = new Document(path,parent);
    }
    ret = m_xlsx->load();
    if(ret)//打开成功则将数据保存到 model 中
    {
        if(model == nullptr)
            model = new QStandardItemModel(parent);
        for(int row = 1;row <= m_xlsx->dimension().rowCount();++ row){
            for(int col = 1;col <= m_xlsx->dimension().columnCount();++ col){
                QVariant value = m_xlsx->read(row,col);
                QStandardItem* item = new QStandardItem(value.toString());
                model->setItem(row-1,col-1,item);
            }
        }
    }
}

QStandardItemModel* OperExcel::getQStandardItemModelPoint()
{
    return model;
}


void OperExcel::save_Excel(QString &path, bool &ret, QObject *parent)
{//保存文件
    // 创建一个新的文档对象
    Document xlsx;
    // 获取要保存的模型数据
    QStandardItemModel *model = getQStandardItemModelPoint();
    if (!model) {
        qWarning() << "Model pointer is null.";
        ret = false;
        return;
    }

    // 获取模型的行数和列数
    int rowCount = model->rowCount();
    int colCount = model->columnCount();

    // 遍历模型中的数据，并将数据写入到 Excel 文件中
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            // 获取模型中的数据项
            QModelIndex index = model->index(row, col);
            QVariant value = model->data(index, Qt::DisplayRole);

            // 将数据写入到 Excel 文件中对应的单元格
            xlsx.write(row + 1, col + 1, value.toString());
        }
    }

    // 保存 Excel 文件
    ret = xlsx.saveAs(path);
    if (!ret) {
        qWarning() << "Failed to save Excel file.";
    }
}
