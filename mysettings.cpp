#include "mysettings.h"

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
    // 在这里可以添加其他初始化代码
}

// 设置示例数据的实现
void MySettings::setExampleData(const QString& data)
{
    m_exampleData = data;
}

// 获取示例数据的实现
QString MySettings::getExampleData() const
{
    return m_exampleData;
}
