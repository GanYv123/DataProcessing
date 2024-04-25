#include "operexcel.h"
#include "QStandardItem"
#include "QStandardItemModel"
#include "QVariant"
#include "QFile"
#include "finalsheet.h"
#include "customdialog.h"
#include "QTemporaryFile"
#include "mainwindow.h"

QXLSX_USE_NAMESPACE            // 添加Xlsx命名空间

#define EXCEL_NAME "./1.xlsx"  // 本demo中用到的excel文件路径文件名


void OperExcel::oper_data_class1(QXlsx::Document &xlsx)
{
    //A2 学年 学期
    QString schoolYear = m_parent_mainWindow->customDialog->get_select_data();

    xlsx.selectSheet(0);  // 设置表 1 既 一班的成绩
    QXlsx::Format xlsx_foramt = xlsx.cellAt(7,1)->format();


    //将 表格数定为各班级人数
    int sourceRow = 7;
    int targetRow = (*course_information)["班级1人数"].toInt()+7-1;
    xlsx.write("A2",schoolYear);

    //课程名称：操作系统  专业：物联网工程  班级： 物联网：21-1  任课老师：xxx,xxx A3
    QString course_info = QString("课程名称：%1 专业：%2 班级：%3 任课老师：%4")
                              .arg((*course_information)["课程名称"].toString())
                              .arg((*course_information)["适用专业"].toString())
                              .arg((*course_information)["班级1"].toString())
                              .arg((*course_information)["授课老师"].toString());
    xlsx.write("A3", course_info);
    //D F H ~5 考勤 作业 实验 占比 B11 C11 D11
    xlsx.write("D5",(*course_information)["rate_kaoqing"].toString().append("%"));
    xlsx.write("F5",(*course_information)["rate_zuoye"].toString().append("%"));
    xlsx.write("H5",(*course_information)["rate_shiyan"].toString().append("%"));


    for(int row = sourceRow;row <= targetRow;++ row){
        xlsx.write(row,1,"202105030141",xlsx_foramt);
        xlsx.write(row,2,"姓名",xlsx_foramt);
        xlsx.write(row,3,"90",xlsx_foramt);
        xlsx.write(row,4,"9",xlsx_foramt);
        xlsx.write(row,5,"40",xlsx_foramt);
        xlsx.write(row,6,"4",xlsx_foramt);
        xlsx.write(row,7,"35",xlsx_foramt);
        xlsx.write(row,8,"32",xlsx_foramt);
        xlsx.write(row,9,"99",xlsx_foramt);
        xlsx.write(row,10,"备注",xlsx_foramt);
    }

}

void OperExcel::oper_data_class2(QXlsx::Document &xlsx)
{
    //A2 学年 学期
    QString schoolYear = m_parent_mainWindow->customDialog->get_select_data();

    xlsx.selectSheet(1);  // 设置表 2 既 2班的成绩
    QXlsx::Format xlsx_foramt = xlsx.cellAt(7,1)->format();


    //将 表格数定为各班级人数
    int sourceRow = 7;
    int targetRow = (*course_information)["班级2人数"].toInt()+7-1;
    xlsx.write("A2",schoolYear);

    //课程名称：操作系统  专业：物联网工程  班级： 物联网：21-2  任课老师：xxx,xxx A3
    QString course_info = QString("课程名称：%1 专业：%2 班级：%3 任课老师：%4")
                              .arg((*course_information)["课程名称"].toString())
                              .arg((*course_information)["适用专业"].toString())
                              .arg((*course_information)["班级2"].toString())
                              .arg((*course_information)["授课老师"].toString());
    xlsx.write("A3", course_info);
    //D F H ~5 考勤 作业 实验 占比 B11 C11 D11
    xlsx.write("D5",(*course_information)["rate_kaoqing"].toString().append("%"));
    xlsx.write("F5",(*course_information)["rate_zuoye"].toString().append("%"));
    xlsx.write("H5",(*course_information)["rate_shiyan"].toString().append("%"));


    for(int row = sourceRow;row <= targetRow;++ row){
        xlsx.write(row,1,"202105030241",xlsx_foramt);
        xlsx.write(row,2,"2姓名",xlsx_foramt);
        xlsx.write(row,3,"90",xlsx_foramt);
        xlsx.write(row,4,"9",xlsx_foramt);
        xlsx.write(row,5,"40",xlsx_foramt);
        xlsx.write(row,6,"4",xlsx_foramt);
        xlsx.write(row,7,"35",xlsx_foramt);
        xlsx.write(row,8,"32",xlsx_foramt);
        xlsx.write(row,9,"99",xlsx_foramt);
        xlsx.write(row,10,"备注",xlsx_foramt);
    }
}

/*
    一班二班分表操作
*/
void OperExcel::fillData(Document& xlsx)
{
    oper_data_class1(xlsx);
    oper_data_class2(xlsx);

}


OperExcel::OperExcel(){}

OperExcel::OperExcel(MainWindow *parent_mainWindow,FinalSheet* finalSheet)
    : m_parent_mainWindow(parent_mainWindow),m_finalSheet(finalSheet){}

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
        //显示课程信息
        FinalSheet::CourseData t_course = m_finalSheet->getCourseData();
        QString courseInfoText = t_course.teacher_name.toString() + t_course.classID.toString();
        m_parent_mainWindow->setLabel_CourseInfo(courseInfoText);
        //读取学生信息


    }
}

QStandardItemModel* OperExcel::getQStandardItemModelPoint()
{
    return m_model;
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

    //填充数据
    fillData(xlsx);

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
    读取表 sheet1 读取到班级1 和 班级 2的 学生信息
*/

void OperExcel::read_StudentInformation()
{
    if(m_xlsx->selectSheet("sheet1")){

    }
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
        QVariant class_1 = m_xlsx->read("D2");
        QVariant class_2 = m_xlsx->read("F2");
        //考勤 作业 实验 占比 B11 C11 D11
        QVariant rate_kaoqing = m_xlsx->read("B11");
        QVariant rate_zuoye = m_xlsx->read("C11");
        QVariant rate_shiyan = m_xlsx->read("D11");
        QVariant students_num1 = m_xlsx->read("E2");
        QVariant students_num2 = m_xlsx->read("G2");

        // 将课程信息插入到 QVariantMap 中
        course_information->insert("课程名称", course_name.toString());
        course_information->insert("授课老师", teacher_name.toString());
        course_information->insert("适用专业", major.toString());
        course_information->insert("课程编号", course_id.toString());
        course_information->insert("学分", credit.toString());
        course_information->insert("课时", hours.toString());
        course_information->insert("班级1",class_1.toString());
        course_information->insert("班级2",class_2.toString());

        course_information->insert("班级1人数",students_num1);
         course_information->insert("班级2人数",students_num2);

        course_information->insert("rate_kaoqing",rate_kaoqing);
        course_information->insert("rate_zuoye",rate_zuoye);
        course_information->insert("rate_shiyan",rate_shiyan);

        FinalSheet::CourseData course;

        course.classID = course_name;
        course.teacher_name = teacher_name;
        course.shoolDays = QVariant(m_parent_mainWindow->customDialog->get_select_data());
        course.rate_attendance = rate_kaoqing;
        course.rate_experiment = rate_shiyan;
        course.rate_homework = rate_zuoye;
        m_finalSheet->setCourseData(course);


    }

}

void OperExcel::setViewModel(QStandardItemModel *o_model)
{
    if(o_model)
        m_model = o_model;
}
