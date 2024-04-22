#ifndef COURSE_H
#define COURSE_H

#include <QString>

class Course
{
public:
    Course();
    Course(const QString &name, const QString &teacher, const QString &major,
           int courseId, int credits, int hours);

    // Getter functions
    QString getName() const;
    QString getTeacher() const;
    QString getMajor() const;
    int getCourseId() const;
    int getCredits() const;
    int getHours() const;

    // Setter functions
    void setName(const QString &name);
    void setTeacher(const QString &teacher);
    void setMajor(const QString &major);
    void setCourseId(int courseId);
    void setCredits(int credits);
    void setHours(int hours);

private:
    QString mName;       // 课程名称 a5
    QString mTeacher;    // 授课老师 b8
    QString mMajor;      // 适用专业 f5
    int mCourseId;       // 课程编号 b5
    int mCredits;        // 学分    e5
    int mHours;          // 课时    d5
};

#endif // COURSE_H
