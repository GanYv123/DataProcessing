#ifndef SQLDATA_H
#define SQLDATA_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include "finalsheet.h"
#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMap>

class SQLData {
public:
    static SQLData& instance();
    bool connect(const QString& host, int port, const QString& user,
                 const QString& password, const QString& dbName);
    void disconnect();
    QSqlQuery executeQuery(const QString& query);
    bool executeUpdate(const QString& query);
    void setLinkedParameter(const QString& host, int port, const QString& user,
                            const QString& password, const QString& dbName);
    bool isLinked();
    QString vectorToJson(const QVector<QVariant>& vector);
    QVector<QVariant> jsonToVector(const QString& jsonString);

    //数据库 建表操作
    void CreateStudentsTable();
    void CreateCourseTable();

    //数据库插入操作
    bool insertStudentData(const FinalSheet::StudentData& student,int classid); //传入 学生 结构体
    bool insertCourseData(const FinalSheet::CourseData& course);

    //读取
    QMap<int,QVector<FinalSheet::StudentData>> readStudentData();
    FinalSheet::CourseData readCourseData();

    //更新数据库
    bool updateSQl(const FinalSheet::StudentData& student,int classid);


private:
    SQLData();
    ~SQLData();
    SQLData(const SQLData&) = delete;
    SQLData& operator=(const SQLData&) = delete;

    QString m_host;
    int m_port;
    QString m_user;
    QString m_password;
    QString m_dbName;
    QString connectionName;
    QSqlDatabase db;  // 声明成员变量 db
};

#endif // SQLDATA_H
