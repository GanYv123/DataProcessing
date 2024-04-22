#include "course.h"

Course::Course()
    : mCourseId(0), mCredits(0), mHours(0)
{
    // 默认构造函数
}

Course::Course(const QString &name, const QString &teacher, const QString &major,
               int courseId, int credits, int hours)
    : mName(name), mTeacher(teacher), mMajor(major),
    mCourseId(courseId), mCredits(credits), mHours(hours)
{
    // 带参数的构造函数，用于初始化成员变量
}

// Getter functions
QString Course::getName() const
{
    return mName;
}

QString Course::getTeacher() const
{
    return mTeacher;
}

QString Course::getMajor() const
{
    return mMajor;
}

int Course::getCourseId() const
{
    return mCourseId;
}

int Course::getCredits() const
{
    return mCredits;
}

int Course::getHours() const
{
    return mHours;
}

// Setter functions
void Course::setName(const QString &name)
{
    mName = name;
}

void Course::setTeacher(const QString &teacher)
{
    mTeacher = teacher;
}

void Course::setMajor(const QString &major)
{
    mMajor = major;
}

void Course::setCourseId(int courseId)
{
    mCourseId = courseId;
}

void Course::setCredits(int credits)
{
    mCredits = credits;
}

void Course::setHours(int hours)
{
    mHours = hours;
}
