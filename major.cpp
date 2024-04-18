#include "major.h"

Major::Major(const QString &name) : m_name(name)
{

}

QString Major::getName() const
{
    return this->m_name;
}

void Major::addStudent(const Student &student)
{
    this->m_students.push_back(student);
}

void Major::addTeacher(const Teacher &teacher)
{
    this->m_teachers.push_back(teacher);
}

QVector<Student> Major::getStudents() const
{
    return this->m_students;
}

QVector<Teacher> Major::getTeachers() const
{
    return this->m_teachers;
}
