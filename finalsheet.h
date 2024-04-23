#ifndef FINALSHEET_H
#define FINALSHEET_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QVariant>

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
    const int start_row_pos = 6;
    const int start_col_pos = 0;
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
