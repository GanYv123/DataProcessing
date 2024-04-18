#include "student.h"


Student::Student(const QString &name
                 , const QString &studentId
                 , const QString &major
                 , const QString &teacherName) :
    m_name(name),m_studentId(studentId),m_major(major)
    ,m_teacherName(teacherName)
{

}

QString Student::getName() const
{
    return this->m_name;
}

QString Student::getStudentId() const
{
    return this->m_studentId;
}

QString Student::getMajor() const
{
    return this->m_major;
}

QString Student::getTeacher() const
{
    return this->m_teacherName;
}

QSet<QString> Student::getAllCourse() const
{
    return this->m_course;
}

void Student::addCourse(const QString &courseName)
{
    if(courseName != "" || courseName != nullptr)
        this->m_course.insert(courseName);
}

bool Student::hasCourse(const QString &courseName) const
{
    return m_course.contains(courseName);
}

double Student::getGrade(const QString &courseName) const
{
    return this->m_courseGrades[courseName];
}
