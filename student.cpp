#include "student.h"

Student::Student(const QString& name, const QString& studentId, const QString& major, const QString& className)
    : m_name(name), m_studentId(studentId), m_major(major), m_className(className)
{
}

QString Student::getName() const
{
    return m_name;
}

QString Student::getStudentId() const
{
    return m_studentId;
}

QString Student::getMajor() const
{
    return m_major;
}

QString Student::getClassName() const
{
    return m_className;
}

void Student::addCourse(const QString& courseName)
{
    m_courses.insert(courseName);
}

bool Student::hasCourse(const QString& courseName) const
{
    return m_courses.contains(courseName);
}

void Student::setExperimentScore(const QString& courseName, const QVariant& score)
{
    m_experimentScores[courseName] = score;
}

void Student::setAttendanceScore(const QString& courseName, const QVariant& score)
{
    m_attendanceScores[courseName] = score;
}

void Student::setHomeworkScore(const QString& courseName, const QVariant& score)
{
    m_homeworkScores[courseName] = score;
}

QVariant Student::getExperimentScore(const QString& courseName) const
{
    return m_experimentScores.value(courseName, QVariant());
}

QVariant Student::getAttendanceScore(const QString& courseName) const
{
    return m_attendanceScores.value(courseName, QVariant());
}

QVariant Student::getHomeworkScore(const QString& courseName) const
{
    return m_homeworkScores.value(courseName, QVariant());
}

QVariant Student::getTotalScore(const QString& courseName) const
{
    QVariant experimentScore = getExperimentScore(courseName);
    QVariant attendanceScore = getAttendanceScore(courseName);
    QVariant homeworkScore = getHomeworkScore(courseName);

    // 这里可以根据需要计算总分，这里只是简单示例
    return experimentScore.toDouble() + attendanceScore.toDouble() + homeworkScore.toDouble();
}
