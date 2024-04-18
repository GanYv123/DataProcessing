#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QMap>
#include <QVector>
#include <QSet>

class Student
{
public:
    Student(const QString& name, const QString& studentId, const QString& major = nullptr,const QString &teacherName = nullptr);

    QString getName() const;
    QString getStudentId() const;
    QString getMajor() const;
    QString getTeacher() const;
    QSet<QString> getAllCourse() const;

    void addCourse(const QString& courseName);
    bool hasCourse(const QString& courseName) const;
    void setGrade(const QString& courseName, double grade);
    double getGrade(const QString& courseName) const;


private:
    QString m_name;
    QString m_studentId;
    QString m_major;
    QString m_teacherName;
    QSet <QString>m_course;
    QMap<QString, double> m_courseGrades; //课程分数
};

#endif // STUDENT_H
