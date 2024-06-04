#ifndef SQLDATA_H
#define SQLDATA_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

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
