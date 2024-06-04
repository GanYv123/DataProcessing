#include "SQLData.h"
#include <QDebug>
#include <QSqlError>
#include <QUuid>

SQLData::SQLData() {
    // 生成唯一的连接名称
    connectionName = QUuid::createUuid().toString();
    qDebug() << "Database connection name:" << connectionName;
}

SQLData::~SQLData() {
    disconnect();
}

SQLData& SQLData::instance() {
    static SQLData sql;
    return sql;
}

bool SQLData::connect(const QString& host, int port, const QString& user, const QString& password, const QString& dbName) {
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase::removeDatabase(connectionName);
    }

    db = QSqlDatabase::addDatabase("QMYSQL", connectionName);

    qDebug() << "Connecting to database with host:" << host
             << ", port:" << port
             << ", user:" << user
             << ", password:" << password
             << ", database:" << dbName
             << ", connection name:" << connectionName;

    db.setHostName(host);
    db.setPort(port);
    db.setUserName(user);
    db.setPassword(password);
    db.setDatabaseName(dbName);

    if (!db.open()) {
        qDebug() << "Failed to connect to database:" << db.lastError().text();
        return false;
    }

    qDebug() << "Connected to database with connection name:" << connectionName;
    return true;
}

QSqlQuery SQLData::executeQuery(const QString& query) {
    QSqlQuery q(db);  // 使用成员变量 db
    if (!q.exec(query)) {
        qDebug() << "Failed to execute query:" << q.lastError().text();
    }
    return q;
}

bool SQLData::executeUpdate(const QString& query) {
    QSqlQuery q(db);  // 使用成员变量 db
    if (!q.exec(query)) {
        qDebug() << "Failed to execute update:" << q.lastError().text();
        return false;
    }
    return true;
}

void SQLData::setLinkedParameter(const QString& host, int port, const QString& user, const QString& password, const QString& dbName) {
    m_host = host;
    m_port = port;
    m_user = user;
    m_password = password;
    m_dbName = dbName;

    qDebug() << "Set database connection parameters:"
             << "host:" << host
             << ", port:" << port
             << ", user:" << user
             << ", password:" << password
             << ", database:" << dbName;
}

bool SQLData::isLinked()
{
    return db.isOpen();
}

void SQLData::disconnect() {
    if (db.isOpen()) {
        db.close();
    }
    QSqlDatabase::removeDatabase(connectionName);
}
