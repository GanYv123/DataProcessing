#include "operexcel.h"
#include "QStandardItem"
#include "QStandardItemModel"
#include "QVariant"
#include "QFile"
#include "finalsheet.h"
#include "customdialog.h"
#include "QTemporaryFile"
#include "mainwindow.h"
#include "cmath"
#include "mysettings.h"
#include "boost/accumulators/accumulators.hpp"
#include "boost/accumulators/statistics.hpp"

QXLSX_USE_NAMESPACE            // 添加Xlsx命名空间

#define EXCEL_NAME "./1.xlsx"  // 本demo中用到的excel文件路径文件名


//往表中写入基本信息
void OperExcel::oper_data_class1(QXlsx::Document &xlsx)
{
    //A2 学年 学期
    QString schoolYear;
    if(m_parent_mainWindow->customDialog->get_select_data() != "")
        schoolYear = m_parent_mainWindow->customDialog->get_select_data();
    else
        schoolYear = m_finalSheet->getCourseData().schoolYears.toString();

    xlsx.selectSheet(0);  // 设置表 1 既 一班的成绩
    QXlsx::Format xlsx_foramt = xlsx.cellAt(7,4)->format();
    QXlsx::Format xlsx_foramt_10point = xlsx.cellAt(7,1)->format();


    //将 表格数定为各班级人数
    int sourceRow = 7;
    int targetRow = m_finalSheet->class1_students().size()+sourceRow-1;
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


    for(int row = sourceRow, index = 0; row <= targetRow; ++row, ++index){
        // 写入学生信息
        xlsx.write(row, 1, m_finalSheet->class1_students().at(index).studentID.toString(), xlsx_foramt_10point);
        xlsx.write(row, 2, m_finalSheet->class1_students().at(index).studentName.toString(), xlsx_foramt_10point);

        // 写入原始考勤成绩
        double attendanceScore = m_finalSheet->class1_students().at(index).attendanceScore.toDouble();
        xlsx.write(row, 3, QString::number((int)attendanceScore), xlsx_foramt);

        // 计算并写入考勤加权成绩
        double weightedAttendanceScore = attendanceScore * (*course_information)["rate_kaoqing"].toDouble() / 100.0;
        xlsx.write(row, 4, QString::number(weightedAttendanceScore, 'f', 2), xlsx_foramt);

        // 写入作业成绩
        double homeworkScore = m_finalSheet->class1_students().at(index).homework.toDouble();
        xlsx.write(row, 5, QString::number((int)homeworkScore), xlsx_foramt);

        // 写入实验成绩
        double experimentScore = m_finalSheet->class1_students().at(index).experiment.toDouble();
        xlsx.write(row, 7, QString::number((int)experimentScore), xlsx_foramt);

        // 计算并写入作业加权成绩
        double weightedHomeworkScore = homeworkScore * (*course_information)["rate_zuoye"].toDouble() / 100.0;
        xlsx.write(row, 6, QString::number(weightedHomeworkScore, 'f', 2), xlsx_foramt);

        // 计算并写入实验加权成绩
        double weightedExperimentScore = experimentScore * (*course_information)["rate_shiyan"].toDouble() / 100.0;
        xlsx.write(row, 8, QString::number(weightedExperimentScore, 'f', 2), xlsx_foramt);

        // 计算并写入总成绩
        double totalScore = weightedAttendanceScore + weightedHomeworkScore + weightedExperimentScore;
        int res = std::round(totalScore);
        xlsx.write(row, 9, QString::number(res), xlsx_foramt);

        // 写入备注
        xlsx.write(row, 10, m_finalSheet->class1_students().at(index).remark, xlsx_foramt);
    }
}

void OperExcel::oper_data_class2(QXlsx::Document &xlsx)
{
    //A2 学年 学期
    QString schoolYear;
    if(m_parent_mainWindow->customDialog->get_select_data() != "")
        schoolYear = m_parent_mainWindow->customDialog->get_select_data();
    else
        schoolYear = m_finalSheet->getCourseData().schoolYears.toString();

    xlsx.selectSheet(1);  // 设置表 2 既 2班的成绩
    QXlsx::Format xlsx_foramt = xlsx.cellAt(7,4)->format();
    QXlsx::Format xlsx_foramt_10point = xlsx.cellAt(7,1)->format();

    //将 表格数定为各班级人数
    int sourceRow = 7;
    int targetRow = m_finalSheet->class2_students().size()+sourceRow-1;
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


    for(int row = sourceRow, index = 0; row <= targetRow; ++row, ++index){
        // 写入学生信息
        xlsx.write(row, 1, m_finalSheet->class2_students().at(index).studentID.toString(), xlsx_foramt_10point);
        xlsx.write(row, 2, m_finalSheet->class2_students().at(index).studentName.toString(), xlsx_foramt_10point);

        // 写入原始考勤成绩
        double attendanceScore = m_finalSheet->class2_students().at(index).attendanceScore.toDouble();
        xlsx.write(row, 3, QString::number((int)attendanceScore), xlsx_foramt);

        // 计算并写入考勤加权成绩
        double weightedAttendanceScore = attendanceScore * (*course_information)["rate_kaoqing"].toDouble() / 100.0;
        xlsx.write(row, 4, QString::number(weightedAttendanceScore, 'f', 2), xlsx_foramt);

        // 写入作业成绩
        double homeworkScore = m_finalSheet->class2_students().at(index).homework.toDouble();
        xlsx.write(row, 5, QString::number((int)homeworkScore), xlsx_foramt);

        // 写入实验成绩
        double experimentScore = m_finalSheet->class2_students().at(index).experiment.toDouble();
        xlsx.write(row, 7, QString::number((int)experimentScore), xlsx_foramt);

        // 计算并写入作业加权成绩
        double weightedHomeworkScore = homeworkScore * (*course_information)["rate_zuoye"].toDouble() / 100.0;
        xlsx.write(row, 6, QString::number(weightedHomeworkScore, 'f', 2), xlsx_foramt);

        // 计算并写入实验加权成绩
        double weightedExperimentScore = experimentScore * (*course_information)["rate_shiyan"].toDouble() / 100.0;
        xlsx.write(row, 8, QString::number(weightedExperimentScore, 'f', 2), xlsx_foramt);

        // 计算并写入总成绩
        double totalScore = weightedAttendanceScore + weightedHomeworkScore + weightedExperimentScore;
        int res = std::round(totalScore);
        xlsx.write(row, 9, QString::number(res), xlsx_foramt);

        // 写入备注
        xlsx.write(row, 10, m_finalSheet->class2_students().at(index).remark, xlsx_foramt);
    }

}

//操作视图
void OperExcel::setClassTableViewModel(QStandardItemModel* &model, int classID)
{
    model->clear();
    if (classID == 1) {
        QVector<FinalSheet::StudentData> class1Students = m_finalSheet->class1_students();
        for (const auto& a : class1Students) {
            QList<QStandardItem*> items; // 创建一个新的items列表
            QStandardItem* item_studentName = new QStandardItem(a.studentName.toString());
            QStandardItem* item_studentID = new QStandardItem(a.studentID.toString());
            QStandardItem* item_attendanceScore = new QStandardItem(a.attendanceScore.toString());
            QStandardItem* item_homework = new QStandardItem(a.homework.toString());
            QStandardItem* item_experiment = new QStandardItem(a.experiment.toString());
            QStandardItem* item_totalScore = new QStandardItem(a.totalScore.toString());
            QStandardItem* item_remark = new QStandardItem(a.remark.toString());

            items.append(item_studentID);
            items.append(item_studentName);
            items.append(item_attendanceScore);
            items.append(item_homework);
            items.append(item_experiment);
            items.append(item_totalScore);
            items.append(item_remark);

            model->appendRow(items);
        }
    }

    if (classID == 2) {
        QVector<FinalSheet::StudentData> class2Students = m_finalSheet->class2_students();
        for (const auto& a : class2Students) {
            QList<QStandardItem*> items; // 创建一个新的items列表
            QStandardItem* item_studentName = new QStandardItem(a.studentName.toString());
            QStandardItem* item_studentID = new QStandardItem(a.studentID.toString());
            QStandardItem* item_attendanceScore = new QStandardItem(a.attendanceScore.toString());
            QStandardItem* item_homework = new QStandardItem(a.homework.toString());
            QStandardItem* item_experiment = new QStandardItem(a.experiment.toString());
            QStandardItem* item_totalScore = new QStandardItem(a.totalScore.toString());
            QStandardItem* item_remark = new QStandardItem(a.remark.toString());

            items.append(item_studentID);
            items.append(item_studentName);
            items.append(item_attendanceScore);
            items.append(item_homework);
            items.append(item_experiment);
            items.append(item_totalScore);
            items.append(item_remark);

            model->appendRow(items);
        }
    }

}

//读取并录入考勤成绩/次数
void OperExcel::setAttendance()
{
    //考勤 成绩位置 学时/2 列 也可直接读取最后一列既为最终成绩列
    if(m_xlsx->selectSheet("1考勤")){
        QVector<FinalSheet::StudentData> class1 = m_finalSheet->class1_students();
        int row = m_xlsx->dimension().rowCount(),col = m_xlsx->dimension().columnCount();
        int attdendance = 0;
        for(int i = 2,index = 0;i <= row;++ i,++ index){
            attdendance = 0;
            for(int j = 3;j < col;++ j){
                attdendance += m_xlsx->read(i,j).toInt();
            }
            class1[index].attendance = attdendance;
            int attScore = (m_finalSheet->getCourseData().attendance_reduce_fractions.toInt()*
                            (m_finalSheet->getCourseData().lessonTime.toInt()/2 -
                             class1.at(index).attendance.toInt()));
            class1[index].attendanceScore = 100-attScore;
        }

        m_finalSheet->setClass1Students(class1);
    }

    if(m_xlsx->selectSheet("考勤2")){
        QVector<FinalSheet::StudentData> class2 = m_finalSheet->class2_students();
        int row = m_xlsx->dimension().rowCount(),col = m_xlsx->dimension().columnCount();
        int attdendance = 0;
        for(int i = 2,index = 0;i <= row;++ i,++ index){
            attdendance = 0;
            for(int j = 3;j < col;++ j){
                attdendance += m_xlsx->read(i,j).toInt();
            }
            class2[index].attendance = attdendance;
            int attScore = 100-(m_finalSheet->getCourseData().attendance_reduce_fractions.toInt()*
                                (m_finalSheet->getCourseData().lessonTime.toInt()/2 -                                                                                                   class2.at(index).attendance.toInt()));
            class2[index].attendanceScore = attScore;
        }

        m_finalSheet->setClass2Students(class2);
    }

}

//加载平时/作业成绩  index(6,19)
void OperExcel::loadHomeworkScore()
{
    //从sheet2中导入平时成绩

    if(m_xlsx->selectSheet("Sheet2")){
        int col,row;
        QVector<FinalSheet::StudentData> studentdatas = m_finalSheet->getAllStudents();
        col = m_xlsx->dimension().columnCount();
        row = m_xlsx->dimension().rowCount();
        QVariant t_sub_homework;
        for(int i = 1;i <= row;++ i){
            for(int j = 6;j <= col;j+=4){
                t_sub_homework = m_xlsx->read(i,j);
                studentdatas[i-1].sub_homework.append(t_sub_homework);
            }
        }
        m_finalSheet->setStudentData(studentdatas);
    }
}

void OperExcel::loadHomeworkByClassSheet()
{
    //从表中导出的数据个数位置后面读
    int choose_COL_Index = 0;
    if(m_xlsx->selectSheet("1平时")){
        choose_COL_Index = 3+m_finalSheet->class1_students().at(0).sub_homework.size(); //样例中位置为第六col
        int row,col;
        row = m_xlsx->dimension().rowCount();
        col = m_xlsx->dimension().columnCount();
        QVector<FinalSheet::StudentData> classDatas1 = m_finalSheet->class1_students();
        for(int i = 6,index = 0; i <= row; ++i,++index) {
            for(int j = choose_COL_Index; j <= col-1; ++j) {
                QVariant t_var = m_xlsx->read(i, j);
                if(t_var.isValid()){
                    bool conversionOk = false;
                    double numericValue = t_var.toDouble(&conversionOk);
                    if(conversionOk) {
                        //qDebug() <<i<<"  "<<j<< "Numeric value:" << numericValue;
                        //将合法数据写入内部数据中
                        classDatas1[index].sub_homework.append(t_var);
                    } else {
                        qDebug() <<i<<"  "<<j<< "Not a numeric value:" << t_var.toString();
                    }
                }
            }
        }
        m_finalSheet->setClass1Students(classDatas1);
    }

    if(m_xlsx->selectSheet("平时2")){
        int row,col;
        row = m_xlsx->dimension().rowCount();
        col = m_xlsx->dimension().columnCount();
        QVector<FinalSheet::StudentData> classDatas2 = m_finalSheet->class2_students();
        for(int i = 6,index = 0; i <= row; ++i,++index) {
            for(int j = choose_COL_Index; j <= col-1; ++j) {
                QVariant t_var = m_xlsx->read(i, j);
                if(t_var.isValid()){
                    bool conversionOk = false;
                    double numericValue = t_var.toDouble(&conversionOk);
                    if(conversionOk) {
                        //qDebug() <<i<<"  "<<j<< "Numeric value:" << numericValue;
                        //将合法数据写入内部数据中
                        classDatas2[index].sub_homework.append(t_var);
                    } else {
                        qDebug() <<i<<"  "<<j<< "Not a numeric value:" << t_var.toString();
                    }
                }
            }
        }
        m_finalSheet->setClass2Students(classDatas2);
    }
}

//计算总成绩
void OperExcel::countTotalScore()
{
    QVector<FinalSheet::StudentData> class1 = m_finalSheet->class1_students();
    QVector<FinalSheet::StudentData> class2 = m_finalSheet->class2_students();
    //计算实验平均成绩
    namespace ba = boost::accumulators;
    using accumulator_type = ba::accumulator_set<double, ba::stats<ba::tag::mean>>;
    //创建一个累加器
    accumulator_type acc;
    for(int i = 0;i < class1.size();++ i){
        // 创建一个新的累加器对象，以清除累加器内容
        acc = accumulator_type();
        for(const auto& a : class1[i].sub_experiment){
            acc(a.toDouble());
        }
        // 将平均值四舍五入到最接近的整数
        int rounded_mean = static_cast<int>(std::round(ba::mean(acc)));
        class1[i].experiment = QVariant(rounded_mean);
    }

    for(int i = 0;i < class2.size();++ i){
        // 创建一个新的累加器对象，以清除累加器内容
        acc = accumulator_type();
        for(const auto& a : class2[i].sub_experiment){
            acc(a.toDouble());
        }
        int rounded_mean = static_cast<int>(std::round(ba::mean(acc)));
        class2[i].experiment = QVariant(rounded_mean);
    }
    /*-----------------------------------------------------------------------------------*/
    //计算作ye成绩平均
    for(int i = 0;i < class1.size();++ i){
        // 创建一个新的累加器对象，以清除累加器内容
        acc = accumulator_type();
        for(const auto& a : class1[i].sub_homework){
            acc(a.toDouble());
        }
        // 将平均值四舍五入到最接近的整数
        int rounded_mean = static_cast<int>(std::round(ba::mean(acc)));
        class1[i].homework = QVariant(rounded_mean);
    }

    for(int i = 0;i < class2.size();++ i){
        // 创建一个新的累加器对象，以清除累加器内容
        acc = accumulator_type();
        for(const auto& a : class2[i].sub_homework){
            acc(a.toDouble());
        }
        int rounded_mean = static_cast<int>(std::round(ba::mean(acc)));
        class2[i].homework = QVariant(rounded_mean);
    }

    ///////////////////////////////////////////////////////
    // 计算班级一的学生总成绩
    for(auto& student : class1){
        double weightedAttendanceScore = student.attendanceScore.toDouble()
                                         * (*course_information)["rate_kaoqing"].toDouble() / 100.0;
        double weightedHomeworkScore = student.homework.toDouble()
                                       * (*course_information)["rate_zuoye"].toDouble() / 100.0;
        double weightedExperimentScore = student.experiment.toDouble()
                                         * (*course_information)["rate_shiyan"].toDouble() / 100.0;
        double totalScore = weightedAttendanceScore + weightedHomeworkScore + weightedExperimentScore;
        student.totalScore = QVariant(static_cast<int>(std::round(totalScore)));
    }

    // 计算班级二的学生总成绩
    for(auto& student : class2){
        double weightedAttendanceScore = student.attendanceScore.toDouble()
                                         * (*course_information)["rate_kaoqing"].toDouble() / 100.0;
        double weightedHomeworkScore = student.homework.toDouble()
                                       * (*course_information)["rate_zuoye"].toDouble() / 100.0;
        double weightedExperimentScore = student.experiment.toDouble()
                                         * (*course_information)["rate_shiyan"].toDouble() / 100.0;
        double totalScore = weightedAttendanceScore + weightedHomeworkScore + weightedExperimentScore;
        student.totalScore = QVariant(static_cast<int>(std::round(totalScore)));
    }
    // 更新班级一和班级二的学生信息
    m_finalSheet->setClass1Students(class1);
    m_finalSheet->setClass2Students(class2);
}

/*
    一班二班分表操作
*/
void OperExcel::fillData(Document& xlsx)
{
    oper_data_class1(xlsx);
    oper_data_class2(xlsx);

}

//构造函数
OperExcel::OperExcel(){}

OperExcel::OperExcel(MainWindow *parent_mainWindow,FinalSheet* finalSheet)
    : m_parent_mainWindow(parent_mainWindow),m_finalSheet(finalSheet){

     if(course_information == nullptr)
        course_information = new QVariantMap();

    // 尝试从配置文件中加载课程信息
    QVariantMap loadedCourseInfo;
    MySettings::instance().loadCourseInformation(loadedCourseInfo);
    if (!loadedCourseInfo.isEmpty()) {
        // 如果配置文件中有课程信息，则使用配置文件中的信息
        *course_information = loadedCourseInfo;
    }
}

void textDemoUnit1(Document& x){
/**
 * @warning 该函数为测试函数
 */
    Q_UNUSED(x);
}

void OperExcel::creat_New_Excel(QString &path, bool &ret)
{
    Document xlsx;
    textDemoUnit1(xlsx);
    ret = xlsx.saveAs(path);
}

//导入文件开始读取信息
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

        read_StudentInformation();       //读取学生信息 同时读取实验信息

        loadHomeworkScore();qDebug()<<"平时成绩已录入";
        //分班学生
        m_finalSheet->splitTableOperation();

        loadHomeworkByClassSheet();

        setAttendance();qDebug()<<"考勤成绩已导入";
        //计算总成绩
        countTotalScore();


        //设置学生信息到tableview
        QStandardItemModel* model1 =  m_parent_mainWindow->getClass1Model();
        QStandardItemModel* model2 =  m_parent_mainWindow->getClass2Model();

        qDebug()<<"一班 信息已导入";
        setClassTableViewModel(model1,1);

        qDebug()<<"二班 信息已导入";
        setClassTableViewModel(model2,2);

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
    if (!xlsx.saveAs(path)) {
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
    读取学生学号及其姓名 还有学习通的 实验成绩
*/
void OperExcel::read_StudentInformation()
{

    if(m_xlsx->selectSheet("Sheet1")){
        int col,row;
        QVector<FinalSheet::StudentData> studentdatas;
        col = m_xlsx->dimension().columnCount();
        row = m_xlsx->dimension().rowCount();

        QVariant t_studentId,t_studentName,t_sub_experiment;
        FinalSheet::StudentData t_studentdata;

        for(int i = 1;i <= row;++ i){
            t_studentName = m_xlsx->read(i,1);
            t_studentId = m_xlsx->read(i,2);

            t_studentdata.studentID = t_studentId;
            t_studentdata.studentName = t_studentName;

            for(int j = 6;j <= col;j+=3){
                t_sub_experiment = m_xlsx->read(i,j);

                t_studentdata.sub_experiment.append(t_sub_experiment);

            }
            studentdatas.append(t_studentdata);
            t_studentdata.sub_experiment.clear();
            //qDebug()<<t_studentdata.studentID<<t_studentdata.studentName;

        }
        m_finalSheet->setStudentData(studentdatas);
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
{
    // 创建 QVariantMap 对象
    if(course_information == nullptr)
        course_information = new QVariantMap();

    // 如果配置文件中没有课程信息，则从 Excel 文件中读取
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
        course.major = major;
        course.rate_homework = rate_zuoye;
        course.attendance_reduce_fractions = m_xlsx->read("B12"); //读取考勤扣分
        course.lessonTime = m_xlsx->read("D5"); //学时
        // A2 B2 C2 2023 - 2024 学年第 1 学期
        int _1 = m_xlsx->read("A2").toInt(),
            _2 = m_xlsx->read("B2").toInt(),
            _3 = m_xlsx->read("C2").toInt();
        QString schoolYears = QString("%1 - %2 学年第 %3 学期").arg(_1).arg(_2).arg(_3);
        course.schoolYears = QVariant(schoolYears);

        m_finalSheet->setCourseData(course);

        // 保存课程信息到配置文件
        MySettings::instance().saveCourseInformation(*course_information);

    }
}


void OperExcel::setViewModel(QStandardItemModel *o_model)
{
    if(o_model)
        m_model = o_model;
}

//设置考勤的模型
void OperExcel::setAttdendanceViewModel(QStandardItemModel *&model)
{
    model->clear();
    int maxSize = -1;
    maxSize = qMax(m_finalSheet->class1_students().size(),m_finalSheet->class2_students().size());

    for(int i = 0;i < maxSize;++ i){
        if(i < m_finalSheet->class1_students().size()){
            model->setItem(i,0,new QStandardItem(m_finalSheet->class1_students().at(i).studentID.toString()));
            model->setItem(i,1,new QStandardItem(m_finalSheet->class1_students().at(i).studentName.toString()));
            model->setItem(i,2,new QStandardItem(m_finalSheet->class1_students().at(i).attendance.toString()));
        }
        if(i < m_finalSheet->class2_students().size()){
            model->setItem(i,3,new QStandardItem(m_finalSheet->class2_students().at(i).studentID.toString()));
            model->setItem(i,4,new QStandardItem(m_finalSheet->class2_students().at(i).studentName.toString()));
            model->setItem(i,5,new QStandardItem(m_finalSheet->class2_students().at(i).attendance.toString()));
        }
    }
}

/**
 * @brief OperExcel::setHomeWorkViewModel
 * @param model 视图模型
 * @todo 传入模型 给模型设置对应的项
 */
void OperExcel::setHomeWorkViewModel(QStandardItemModel *&model,int classID)
{
    if(model == nullptr) return;

    if(classID == 1){
        QVector<FinalSheet::StudentData> studentData = m_finalSheet->class1_students();
        for(int i = 0;i < studentData.size();++ i){
            QVector<QVariant> sub_hom = studentData.at(i).sub_homework;
            model->setItem(i,0,new QStandardItem(studentData.at(i).studentID.toString()));
            model->setItem(i,1,new QStandardItem(studentData.at(i).studentName.toString()));
            for(int j = 0,index = 2;j < studentData.at(i).sub_homework.size();++j,++index){
                model->setItem(i,index,new QStandardItem(sub_hom.at(j).toString()));
            }
            model->setItem(i,studentData.at(i).sub_homework.size()+2,
                           new QStandardItem(studentData.at(i).homework.toString()));
        }

    }else if(classID == 2){
        QVector<FinalSheet::StudentData> studentData = m_finalSheet->class2_students();
        for(int i = 0;i < studentData.size();++ i){
            QVector<QVariant> sub_hom = studentData.at(i).sub_homework;
            model->setItem(i,0,new QStandardItem(studentData.at(i).studentID.toString()));
            model->setItem(i,1,new QStandardItem(studentData.at(i).studentName.toString()));
            for(int j = 0,index = 2;j < studentData.at(i).sub_homework.size();++j,++index){
                model->setItem(i,index,new QStandardItem(sub_hom.at(j).toString()));
            }
            model->setItem(i,studentData.at(i).sub_homework.size()+2,
                           new QStandardItem(studentData.at(i).homework.toString()));
        }
    }
    model->setHorizontalHeaderItem(m_finalSheet->class1_students()[0].
                                       sub_homework.size()+2,new QStandardItem("总成绩"));

}

void OperExcel::setExperimentViewModel(QStandardItemModel *&model,int classID)
{
    if(model == nullptr) return;

    if(classID == 1){
        QVector<FinalSheet::StudentData> studentData = m_finalSheet->class1_students();
        for(int i = 0;i < studentData.size();++ i){
            QVector<QVariant> sub_exp = studentData.at(i).sub_experiment;
            model->setItem(i,0,new QStandardItem(studentData.at(i).studentID.toString()));
            model->setItem(i,1,new QStandardItem(studentData.at(i).studentName.toString()));
            for(int j = 0,index = 2;j < studentData.at(i).sub_experiment.size();++j,++index){
                model->setItem(i,index,new QStandardItem(sub_exp.at(j).toString()));
            }
            model->setItem(i,studentData.at(i).sub_experiment.size()+2,
                           new QStandardItem(studentData.at(i).experiment.toString()));
        }

    }else if(classID == 2){
        QVector<FinalSheet::StudentData> studentData = m_finalSheet->class2_students();
        for(int i = 0;i < studentData.size();++ i){
            QVector<QVariant> sub_exp = studentData.at(i).sub_experiment;
            model->setItem(i,0,new QStandardItem(studentData.at(i).studentID.toString()));
            model->setItem(i,1,new QStandardItem(studentData.at(i).studentName.toString()));
            for(int j = 0,index = 2;j < studentData.at(i).sub_experiment.size();++j,++index){
                model->setItem(i,index,new QStandardItem(sub_exp.at(j).toString()));
            }
            model->setItem(i,studentData.at(i).sub_experiment.size()+2,
                           new QStandardItem(studentData.at(i).experiment.toString()));
        }
    }
    model->setHorizontalHeaderItem(m_finalSheet->class1_students()[0].
                                       sub_experiment.size()+2,new QStandardItem("总成绩"));
}
