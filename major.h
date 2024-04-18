#ifndef MAJOR_H
#define MAJOR_H

#include <student.h>
#include <teacher.h>
#include <QVector>

class Major
{
public:
    Major(const QString& name);

    QString getName() const;

    void addStudent(const Student& student);
    void addTeacher(const Teacher& teacher);
    QVector<Student> getStudents() const;
    QVector<Teacher> getTeachers() const;
private:
    QString m_name;
    QVector<Student> m_students;
    QVector<Teacher> m_teachers;
};

#endif // MAJOR_H
