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

bool SQLData::insertStudentData(const FinalSheet::StudentData &student)
{//数据库中插入学生信息
    QString insertQuery = QString(
        "INSERT INTO students(studentID, "
        "studentName, attendance, attendanceScore, "
        "homework, sub_homework, experiment, sub_experiment, totalScore, remark)"
        "VALUES ('%1', '%2', %3, %4, %5, '%6', %7, '%8', %9, '%10')")
        .arg(student.studentID.toString())
        .arg(student.studentName.toString())
        .arg(student.attendance.toInt())
        .arg(student.attendanceScore.toDouble())
        .arg(student.homework.toDouble())
        .arg(vectorToJson(student.sub_homework))
        .arg(student.experiment.toDouble())
        .arg(vectorToJson(student.sub_experiment))
        .arg(student.totalScore.toDouble())
        .arg(student.remark.toString());

    return executeUpdate(insertQuery);
}

QVector<FinalSheet::StudentData> SQLData::readStudentData()
{
    QVector<FinalSheet::StudentData> studentDataList;

    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return studentDataList;
    }

    QString selectQuery = "SELECT * FROM students";
    QSqlQuery query = executeQuery(selectQuery); // 使用 executeQuery 函数执行查询

    while (query.next()) {
        FinalSheet::StudentData student;
        student.studentID = query.value("studentID");
        student.studentName = query.value("studentName");
        student.attendance = query.value("attendance");
        student.attendanceScore = query.value("attendanceScore");
        student.homework = query.value("homework");
        student.sub_homework = jsonToVector(query.value("sub_homework").toString());
        student.experiment = query.value("experiment");
        student.sub_experiment = jsonToVector(query.value("sub_experiment").toString());
        student.totalScore = query.value("totalScore");
        student.remark = query.value("remark");

        studentDataList.append(student);
    }

    qDebug() << "Number of students read:" << studentDataList.size();
    return studentDataList;
}

// vectorToJson 函数的定义
QString SQLData::vectorToJson(const QVector<QVariant>& vector)
{
    QJsonArray jsonArray;
    for (const QVariant& item : vector) {
        jsonArray.append(QJsonValue::fromVariant(item));
    }
    QJsonDocument jsonDoc(jsonArray);
    return QString(jsonDoc.toJson(QJsonDocument::Compact));
}
QVector<QVariant> SQLData::jsonToVector(const QString& jsonString)
{
    QVector<QVariant> vector;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();
        for (const QJsonValue& value : jsonArray) {
            vector.append(value.toVariant());
        }
    }
    return vector;
}



void SQLData::disconnect() {
    if (db.isOpen()) {
        db.close();
    }
    QSqlDatabase::removeDatabase(connectionName);
}
