#include "finalsheet.h"
#include "mysettings.h"

// 构造函数
FinalSheet::FinalSheet(QObject *parent)
    : QObject(parent)
{
    // 初始化
}

// 设置学生数据
void FinalSheet::setStudentData(const QVector<StudentData>& students)
{
    m_students = students;

    // 数据发生改变，发射 dataChanged() 信号
    emit dataChanged();
}

void FinalSheet::setCourseData(const CourseData &course)
{
    m_courseData = course;
    emit dataChanged();
}

QVector<FinalSheet::StudentData> FinalSheet::getStudentData(QString classID) const
{
    return m_StudentDataForClass[classID];
}

QMap<QString, QVector<FinalSheet::StudentData> > FinalSheet::getStudentData() const
{
    return m_StudentDataForClass;
}


FinalSheet::CourseData FinalSheet::getCourseData() const
{
    return m_courseData;
}

void FinalSheet::splitTableOperation()
{
    // 确保学生列表非空
    if (m_students.empty()) return;

    for (auto& a : m_students) {
        // 使用引用避免不必要的复制
        if (a.studentID.toString().at(9) == '1') {
            // 1班
            m_class1_students.append(a);
        } else if (a.studentID.toString().at(9) == '2') {
            // 2班
            m_class2_students.append(a);
        } else {
            // 其他操作
        }
    }

    // 输出分班结果
    qDebug() << QString("检测到1班学生 %1 个").arg(m_class1_students.size());
    qDebug() << QString("检测到2班学生 %1 个").arg(m_class2_students.size());
}

QVector<FinalSheet::StudentData> FinalSheet::getAllStudents()
{
    return m_students;
}

QVector<FinalSheet::StudentData> FinalSheet::class1_students() const
{
    return m_class1_students;
}

QVector<FinalSheet::StudentData> FinalSheet::class2_students() const
{
    return m_class2_students;
}

void FinalSheet::setClass1Students(const QVector<FinalSheet::StudentData> &students)
{
    m_class1_students = students;
}

void FinalSheet::setClass2Students(const QVector<FinalSheet::StudentData> &students)
{
    m_class2_students = students;
}
/**
 * @brief FinalSheet::setclass1Config
 * @target 将班级里的学生信息保存到配置文件
 */
void FinalSheet::setclass1Config()
{
    MySettings::instance().saveStudentData("class1",m_class1_students);

}

void FinalSheet::setclass2Config()
{
    MySettings::instance().saveStudentData("class2",m_class2_students);
}
/** 读取信息
 * @brief FinalSheet::readclass1FromConfig
 */
void FinalSheet::readclass1FromConfig()
{
    MySettings::instance().loadStudentData("class1",m_class1_students);
}

void FinalSheet::readclass2FromConfig()
{
        MySettings::instance().loadStudentData("class2",m_class2_students);
}

void FinalSheet::setCourseDataConfig()
{
    MySettings::instance().saveCourseData("CourseData",m_courseData);
}

void FinalSheet::readCourseDataConfig()
{
    MySettings::instance().loadCourseData("CourseData",m_courseData);
}

/*
    考勤 C4 -> B10 rate D5
    作业 rate F5
    实验 rate H5

*/
