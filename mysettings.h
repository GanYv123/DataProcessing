#ifndef MYSETTINGS_H
#define MYSETTINGS_H

#include <QString>
#include "finalsheet.h"
#include "QStandardItem"

class MySettings {
public:
    static MySettings& instance();

    void setExampleData(const QString& data);
    QString getExampleData() const;

    //保存学生数据
    void saveStudentData(const QString& key,const QVector<FinalSheet::StudentData>& students);
    void loadStudentData(const QString& key,QVector<FinalSheet::StudentData>& students);

    //保存课程信息
    void loadCourseInformation(QVariantMap &courseInfo);
    void saveCourseInformation(const QVariantMap &courseInfo);

    // 保存课程信息
    void saveCourseData(const QString& key, const FinalSheet::CourseData& course);
    void loadCourseData(const QString& key, FinalSheet::CourseData& course);

    //隐藏配置文件
    void hideFile();
    void unhideFile();

    //配置文件的状态
    void saveFile_status();
    void loadFile_status();

    bool getIshided() const;
    void setIshided(bool newIshided);

    bool configExists();

    QString getCONFIG_FILE_NAME() const;
    void setCONFIG_FILE_NAME(const QString &newCONFIG_FILE_NAME);

    // 选择配置文件保存路径
    void selectConfigFilePath();


private:
    MySettings();
    ~MySettings() = default;

    MySettings(const MySettings&) = delete;
    MySettings& operator=(const MySettings&) = delete;

    QString m_exampleData;

    void loadSettings();
    void saveSettings() const;

    bool ishided = false; //是否隐藏
    QString CONFIG_FILE_NAME = "dataproconfig.ini";
};

#endif // MYSETTINGS_H
