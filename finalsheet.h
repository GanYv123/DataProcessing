#ifndef FINALSHEET_H
#define FINALSHEET_H

#include <QObject>
#include <QString>
#include <QVector>

class FinalSheet : public QObject
{
    Q_OBJECT

public:
    // 内部结构定义
    struct StudentData {
        QString studentID;
        QString studentName;
        double attendance;  // 考勤成绩
        double homework;    // 作业成绩
        double experiment;  // 实验成绩
        double totalScore;  // 总成绩
        QString remark;
    };

    // 构造函数
    explicit FinalSheet(QObject *parent = nullptr);

    // 设置学生数据
    void setStudentData(const QVector<StudentData>& students);

    // 获取学生数据
    QVector<StudentData> getStudentData() const;

signals:
    // 定义信号（可根据需要添加）
    void dataChanged();

private:
    QVector<StudentData> m_students;
};

#endif // FINALSHEET_H
