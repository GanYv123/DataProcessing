#ifndef MYSETTINGS_H
#define MYSETTINGS_H

#include <QString>  // 包含需要的头文件

class MySettings
{
public:
    // 获取单例实例的静态成员函数
    static MySettings& instance();

    // 设置和获取示例数据的成员函数
    void setExampleData(const QString& data);
    QString getExampleData() const;

private:
    // 私有构造函数，防止外部创建实例
    MySettings();

    // 数据成员，用于存储示例数据
    QString m_exampleData;
};

#endif // MYSETTINGS_H
