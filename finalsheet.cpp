#include "finalsheet.h"

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

QVector<FinalSheet::StudentData> FinalSheet::class1_students() const
{
    return m_class1_students;
}

QVector<FinalSheet::StudentData> FinalSheet::class2_students() const
{
    return m_class2_students;
}



/*
    考勤 C4 -> B10 rate D5
    作业 rate F5
    实验 rate H5



*/
