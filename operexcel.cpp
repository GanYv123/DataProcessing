#include "operexcel.h"
#include "QStandardItem"
#include "QStandardItemModel"
#include "QVariant"
#include "QFile"
#include "QTemporaryFile"

QXLSX_USE_NAMESPACE            // 添加Xlsx命名空间

#define EXCEL_NAME "./1.xlsx"  // 本demo中用到的excel文件路径文件名

OperExcel::OperExcel()
{

}

void textDemoUnit1(Document& x){
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
        //如果打开成功则读取文件里面的 基本课程信息
        this->read_course_information();

    }
}

QStandardItemModel* OperExcel::getQStandardItemModelPoint()
{
    return model;
}


void OperExcel::save_Excel(QString &path, bool &ret, QObject *parent)
{//保存文件 将数据序列化

}

void OperExcel::export_Excel(QString &path, bool &ret, QObject *parent)
{//导出表格
    //读取资源文件中的excel表格到临时文件中
    QByteArray fileData;
    QString resourcePath = ":/excel_formwork/formwork.xlsx";//qrc:/excel_formwork/formwork.xlsx
    QTemporaryFile tempFile;

    if(!tempFile.open()){
        qWarning()<<"failed to create temporary file";
        ret = false;
        return;
    }

    //创建好了临时文件 从资源文件中读取文件并写入到临时文件
    QFile resourceFile(resourcePath);
    if(resourceFile.open(QIODevice::ReadOnly)){
        fileData = resourceFile.readAll();
        tempFile.write(fileData);
        resourceFile.close();
    }else{
        qWarning()<<"failed to read resource file";
        tempFile.close();
        ret = false;
        return;
    }
    tempFile.close();
    //关闭临时文件 使用 qxslx加载临时文件
    QString tempFilePath = tempFile.fileName();
    Document xlsx(tempFilePath,parent);
    if (!xlsx.load()) {
        qDebug() << "Failed to load Excel file";
        ret = false;
        return;
    }

    // 保存 Excel 文件到指定路径
    if (!xlsx.saveAs("111test.xlsx")) {
        qDebug() << "Failed to save Excel file";
        ret = false;
        return;
    }

    // 设置成功状态
    ret = true;

}

QVariantMap *OperExcel::get_course_information()
{//返回课程的信息
    return this->course_information;
}

/*
// 课程名称 a5
// 授课老师 b8
// 适用专业 f5
// 课程编号 b5
// 学分    e5
// 课时    d5
// 过程性考试比例
   考勤    10
   作业    30
   实验    60
//总评比例：

*/

void OperExcel::read_course_information()
{//读取课程信息
course_information = new QVariantMap(); // 创建 QVariantMap 对象

    if (m_xlsx->selectSheet("概述")) { // 选择指定的工作表
        // 从指定单元格读取课程信息
        QVariant course_name = m_xlsx->read("A5");
        QVariant teacher_name = m_xlsx->read("B8");
        QVariant major = m_xlsx->read("F5");
        QVariant course_id = m_xlsx->read("B5");
        QVariant credit = m_xlsx->read("E5");
        QVariant hours = m_xlsx->read("D5");

        // 将课程信息插入到 QVariantMap 中
        course_information->insert("课程名称", course_name.toString());
        course_information->insert("授课老师", teacher_name.toString());
        course_information->insert("适用专业", major.toString());
        course_information->insert("课程编号", course_id.toString());
        course_information->insert("学分", credit.toString());
        course_information->insert("课时", hours.toString());

        // 打印课程名称，用于调试
        qDebug() << "课程名称：" << course_information->value("课程名称").toString();
        qDebug() << "授课老师" << course_information->value("授课老师").toString();
        qDebug() << "适用专业" << course_information->value("适用专业").toString();
        qDebug() << "课程编号" << course_information->value("课程编号").toString();
        qDebug() << "学分" << course_information->value("学分").toString();
        qDebug() << "课时" << course_information->value("课时").toString();

    }

}
