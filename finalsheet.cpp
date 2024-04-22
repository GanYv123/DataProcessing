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

// 获取学生数据
QVector<FinalSheet::StudentData> FinalSheet::getStudentData() const
{
    return m_students;
}

/*
    考勤 C4 -> B10 rate D5
    作业 rate F5
    实验 rate H5



*/
