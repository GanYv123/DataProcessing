#include "mysettings.h"
#include <QSettings>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>

#ifdef Q_OS_WIN // 如果是 Windows 平台
#include <Windows.h> // 包含Windows API头文件
#endif


const QString CONFIG_FILE_NAME = "config.ini";

// 定义静态成员变量
MySettings& MySettings::instance()
{
    static MySettings instance;
    return instance;
}

// 私有构造函数，初始化示例数据
MySettings::MySettings()
    : m_exampleData("Default Data") // 默认示例数据
{
    QCoreApplication::setOrganizationName("LZIT");
    QCoreApplication::setOrganizationDomain("https://github.com/GanYv123/DataProcessing");
    QCoreApplication::setApplicationName("成绩统计");

    loadSettings(); // 在构造函数中加载配置
}

// 设置示例数据的实现
void MySettings::setExampleData(const QString& data)
{
    m_exampleData = data;
    saveSettings(); // 每次设置数据后保存配置
}

// 获取示例数据的实现
QString MySettings::getExampleData() const
{
    return m_exampleData;
}

void MySettings::loadCourseInformation(QVariantMap &courseInfo)
{
    QSettings settings(CONFIG_FILE_NAME, QSettings::IniFormat);
    settings.beginGroup("CourseInfo");
    QStringList keys = settings.childKeys();
    foreach (const QString &key, keys) {
        courseInfo[key] = settings.value(key);
    }
    settings.endGroup();
}

void MySettings::saveCourseInformation(const QVariantMap &courseInfo)
{
    QSettings settings(CONFIG_FILE_NAME, QSettings::IniFormat);
    settings.beginGroup("CourseInfo");
    for (auto it = courseInfo.begin(); it != courseInfo.end(); ++it) {
        settings.setValue(it.key(), it.value());
    }
    settings.endGroup();
}


void MySettings::saveStudentData(const QString &key, const QVector<FinalSheet::StudentData> &students)
{
    QSettings settings(CONFIG_FILE_NAME, QSettings::IniFormat);
    QVariantList studentList;
    for (const FinalSheet::StudentData& student : students) {
        QVariantMap studentMap;
        studentMap["studentID"] = student.studentID;
        studentMap["studentName"] = student.studentName;
        studentMap["attendance"] = student.attendance;
        studentMap["attendanceScore"] = student.attendanceScore;
        studentMap["homework"] = student.homework;
        studentMap["sub_homework"] = QVariant::fromValue(student.sub_homework);
        studentMap["experiment"] = student.experiment;
        studentMap["sub_experiment"] = QVariant::fromValue(student.sub_experiment);
        studentMap["totalScore"] = student.totalScore;
        studentMap["remark"] = student.remark;
        studentList.append(studentMap);
    }
    settings.setValue(key, studentList);
}

void MySettings::loadStudentData(const QString &key, QVector<FinalSheet::StudentData> &students)
{
    QSettings settings(CONFIG_FILE_NAME, QSettings::IniFormat);
    QVariantList studentList = settings.value(key).toList();
    students.clear();
    for (const QVariant &studentVariant : studentList) {
        QVariantMap studentMap = studentVariant.toMap();
        FinalSheet::StudentData student;
        student.studentID = studentMap["studentID"].toString();
        student.studentName = studentMap["studentName"].toString();
        student.attendance = studentMap["attendance"].toInt();
        student.attendanceScore = studentMap["attendanceScore"].toInt();
        student.homework = studentMap["homework"].toInt();
        student.sub_homework = studentMap["sub_homework"].toList();
        student.experiment = studentMap["experiment"].toInt();
        student.sub_experiment = studentMap["sub_experiment"].toList();
        student.totalScore = studentMap["totalScore"].toInt();
        student.remark = studentMap["remark"].toString();
        students.append(student);
    }
}


void MySettings::saveCourseData(const QString& key, const FinalSheet::CourseData& course) {
    QSettings settings(CONFIG_FILE_NAME, QSettings::IniFormat);
    settings.setValue(key + "/teacher_name", course.teacher_name);
    settings.setValue(key + "/shoolDays", course.shoolDays);
    settings.setValue(key + "/major", course.major);
    settings.setValue(key + "/classID", course.classID);
    settings.setValue(key + "/rate_attendance", course.rate_attendance);
    settings.setValue(key + "/rate_homework", course.rate_homework);
    settings.setValue(key + "/rate_experiment", course.rate_experiment);
    settings.setValue(key + "/attendance_reduce_fractions", course.attendance_reduce_fractions);
    settings.setValue(key + "/lessonTime", course.lessonTime);
    settings.setValue(key + "/schoolYears", course.schoolYears);
}

void MySettings::loadCourseData(const QString& key, FinalSheet::CourseData& course) {
    QSettings settings(CONFIG_FILE_NAME, QSettings::IniFormat);
    course.teacher_name = settings.value(key + "/teacher_name");
    course.shoolDays = settings.value(key + "/shoolDays");
    course.major = settings.value(key + "/major");
    course.classID = settings.value(key + "/classID");
    course.rate_attendance = settings.value(key + "/rate_attendance");
    course.rate_homework = settings.value(key + "/rate_homework");
    course.rate_experiment = settings.value(key + "/rate_experiment");
    course.attendance_reduce_fractions = settings.value(key + "/attendance_reduce_fractions");
    course.lessonTime = settings.value(key + "/lessonTime");
    course.schoolYears = settings.value(key + "/schoolYears");
}

void MySettings::hideFile()
{
#ifdef Q_OS_WIN // 如果是 Windows 平台
    const QString filePath = CONFIG_FILE_NAME.toUtf8(); // 将QString转换为标准字符串
    LPCWSTR filePathWide = (LPCWSTR)filePath.utf16(); // 将标准字符串转换为宽字符串


    this->ishided = true;
    saveFile_status();

    // 设置文件属性为隐藏
    if (SetFileAttributes(filePathWide, FILE_ATTRIBUTE_HIDDEN)) {
        qDebug() << "文件" << filePath << "已隐藏";
    } else {
        qDebug() << "无法隐藏文件" << filePath;
    }
#else // 如果是其他平台
    qDebug() << "该功能仅适用于 Windows 平台";
#endif
}

void MySettings::unhideFile()
{
#ifdef Q_OS_WIN // 如果是 Windows 平台
    const QString filePath = CONFIG_FILE_NAME.toUtf8(); // 将QString转换为标准字符串
    LPCWSTR filePathWide = (LPCWSTR)filePath.utf16(); // 将标准字符串转换为宽字符串

    // 取消隐藏后保存文件状态
    this->ishided = false;
    saveFile_status();
    // 移除文件的隐藏属性

    if (SetFileAttributes(filePathWide, FILE_ATTRIBUTE_NORMAL)) {
        qDebug() << "文件" << filePath << "取消隐藏";
    } else {
        qDebug() << "无法取消隐藏文件" << filePath;
    }
#else // 如果是其他平台
    qDebug() << "该功能仅适用于 Windows 平台";
#endif
}


void MySettings::saveFile_status()
{
    QSettings settings(CONFIG_FILE_NAME, QSettings::IniFormat);
    settings.setValue("hide_Config", ishided);
}

void MySettings::loadFile_status()
{
    QSettings settings(CONFIG_FILE_NAME, QSettings::IniFormat);
    this->ishided = settings.value("hide_Config").toBool();
}

// 加载配置
void MySettings::loadSettings()
{
    loadFile_status();
}

// 保存配置
void MySettings::saveSettings() const
{
    QSettings settings(CONFIG_FILE_NAME, QSettings::IniFormat);
    settings.setValue("Example/ExampleData", m_exampleData);
}

bool MySettings::getIshided() const
{
    return ishided;
}

void MySettings::setIshided(bool newIshided)
{
    ishided = newIshided;
}
