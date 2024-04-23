#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QMap>
#include <QSet>
#include <QVariant>

class Student
{
public:
    Student(const QString& name, const QString& studentId, const QString& major = QString(), const QString& className = QString());

    QString getName() const;
    QString getStudentId() const;
    QString getMajor() const;
    QString getClassName() const;

    void addCourse(const QString& courseName);
    bool hasCourse(const QString& courseName) const;
    void setExperimentScore(const QString& courseName, const QVariant& score);
    void setAttendanceScore(const QString& courseName, const QVariant& score);
    void setHomeworkScore(const QString& courseName, const QVariant& score);

    QVariant getExperimentScore(const QString& courseName) const;
    QVariant getAttendanceScore(const QString& courseName) const;
    QVariant getHomeworkScore(const QString& courseName) const;
    QVariant getTotalScore(const QString& courseName) const;

private:
    QString m_name;
    QString m_studentId;
    QString m_major;
    QString m_className;
    QSet<QString> m_courses;
    QMap<QString, QVariant> m_experimentScores;
    QMap<QString, QVariant> m_attendanceScores;
    QMap<QString, QVariant> m_homeworkScores;
};

#endif // STUDENT_H
