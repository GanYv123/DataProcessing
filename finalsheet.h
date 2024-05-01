#ifndef FINALSHEET_H
#define FINALSHEET_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QVariant>
#include <QMap>

class FinalSheet : public QObject
{
    Q_OBJECT

public:
    // 内部结构定义
    struct StudentData {
        QVariant studentID;
        QVariant studentName;
        QVariant attendance;  // 考勤成绩
        QVariant homework;    // 作业成绩
        QVariant experiment;  // 实验成绩
        QVariant totalScore;  // 总成绩
        QVariant remark;
    };

    struct CourseData{
        QVariant teacher_name;
        QVariant shoolDays;
        QVariant major;
        QVariant classID;
        QVariant rate_attendance;
        QVariant rate_homework;
        QVariant rate_experiment;
    };


    const int start_row_pos = 6;
    const int start_col_pos = 0;
    // 构造函数
    explicit FinalSheet(QObject *parent = nullptr);

    // 设置学生数据
    void setStudentData(const QVector<StudentData>& students);
    //设置课程信息
    void setCourseData(const CourseData& course);


    // 获取学生数据
    QVector<StudentData> getStudentData(QString classID) const;
    QMap<QString,QVector<FinalSheet::StudentData>> getStudentData() const;

    //获取课程信息
    CourseData getCourseData() const;

signals:
    // 定义信号（可根据需要添加）
    void dataChanged();

private:
    QVector<StudentData> m_students;
    CourseData m_courseData;
    QMap<QString,QVector<FinalSheet::StudentData>> m_StudentDataForClass;
    //studentDataForClass["通信工程21-1"](QVector<FinalSheet::StudentData>)
};

#endif // FINALSHEET_H
