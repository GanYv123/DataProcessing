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
        QVariant studentID = QVariant("0");
        QVariant studentName = QVariant("Null");
        QVariant attendance = QVariant(0);  // 考勤次数
        QVariant attendanceScore = QVariant(0);  // 考勤成绩

        QVariant homework = QVariant(0);    // 作业成绩
        QVector<QVariant> sub_homework;       //分次作业成绩

        QVariant experiment = QVariant(0);  // 实验成绩
        QVector<QVariant> sub_experiment;     // 分次实验成绩

        QVariant totalScore = QVariant(0);  // 总成绩
        QVariant remark = QVariant(" ");

    };

    struct CourseData{
        QVariant teacher_name = QVariant();
        QVariant shoolDays = QVariant();
        QVariant major = QVariant();
        QVariant classID = QVariant();
        QVariant rate_attendance = QVariant();
        QVariant rate_homework = QVariant();
        QVariant rate_experiment = QVariant();
        QVariant attendance_reduce_fractions = QVariant(); //考勤扣分
        QVariant lessonTime = QVariant();//课时
        QVariant schoolYears = QVariant();
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

    //分表操作
    void splitTableOperation();

    QVector<FinalSheet::StudentData> getAllStudents();

    QVector<FinalSheet::StudentData> class1_students() const;
    QVector<FinalSheet::StudentData> class2_students() const;
    void setClass1Students(const QVector<FinalSheet::StudentData> &students);
    void setClass2Students(const QVector<FinalSheet::StudentData> &students);

    void setclass1Config();
    void setclass2Config();
    void readclass1FromConfig();
    void readclass2FromConfig();
    void setCourseDataConfig();
    void readCourseDataConfig();


signals:
    // 定义信号（可根据需要添加）
    void dataChanged();

private:
    QVector<StudentData> m_students;
    CourseData m_courseData;
    QMap<QString,QVector<FinalSheet::StudentData>> m_StudentDataForClass;
    //studentDataForClass["通信工程21-1"](QVector<FinalSheet::StudentData>)

    QVector<FinalSheet::StudentData> m_class1_students;
    QVector<FinalSheet::StudentData> m_class2_students;
};

#endif // FINALSHEET_H
