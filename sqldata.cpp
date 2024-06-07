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

bool SQLData::insertStudentData(const FinalSheet::StudentData &student,int classid)
{//数据库中插入学生信息
    QString insertQuery = QString(
        "INSERT INTO students(studentID, "
        "studentName, attendance, attendanceScore, "
        "homework, sub_homework, experiment, sub_experiment, totalScore, remark, classid)"
        "VALUES ('%1', '%2', %3, %4, %5, '%6', %7, '%8', %9, '%10', %11)")
        .arg(student.studentID.toString())
        .arg(student.studentName.toString())
        .arg(student.attendance.toInt())
        .arg(student.attendanceScore.toDouble())
        .arg(student.homework.toDouble())
        .arg(vectorToJson(student.sub_homework))
        .arg(student.experiment.toDouble())
        .arg(vectorToJson(student.sub_experiment))
        .arg(student.totalScore.toDouble())
        .arg(student.remark.toString())
        .arg(classid);

    return executeUpdate(insertQuery);
}

bool SQLData::insertCourseData(const FinalSheet::CourseData &course) {
    // 构建参数化查询
    QString insertQuery = R"(
        INSERT INTO CourseData (
            teacher_name,
            shoolDays,
            major,
            classID,
            rate_attendance,
            rate_homework,
            rate_experiment,
            attendance_reduce_fractions,
            lessonTime,
            schoolYears
        )
        VALUES (
            :teacher_name,
            :shoolDays,
            :major,
            :classID,
            :rate_attendance,
            :rate_homework,
            :rate_experiment,
            :attendance_reduce_fractions,
            :lessonTime,
            :schoolYears
        )
    )";

    // 执行参数化查询
    QSqlQuery query(db);
    query.prepare(insertQuery);

    // 绑定参数
    query.bindValue(":teacher_name", course.teacher_name);
    query.bindValue(":shoolDays", course.shoolDays);
    query.bindValue(":major", course.major);
    query.bindValue(":classID", course.classID);
    query.bindValue(":rate_attendance", course.rate_attendance);
    query.bindValue(":rate_homework", course.rate_homework);
    query.bindValue(":rate_experiment", course.rate_experiment);
    query.bindValue(":attendance_reduce_fractions", course.attendance_reduce_fractions);
    query.bindValue(":lessonTime", course.lessonTime);
    query.bindValue(":schoolYears", course.schoolYears);

    // 执行查询
    if (!query.exec()) {
        qDebug() << "Failed to insert course data:" << query.lastError();
        return false;
    }

    return true;
}

FinalSheet::CourseData SQLData::readCourseData() {
    FinalSheet::CourseData course;

    // 执行查询
    QString selectQuery = "SELECT * FROM CourseData";
    QSqlQuery query = executeQuery(selectQuery);

    // 检查查询是否执行成功
    if (!query.exec()) {
        qDebug() << "Failed to read course data:" << query.lastError();
        return course;
    }

    // 处理结果集
    if (query.next()) {
        course.teacher_name = query.value("teacher_name");
        course.shoolDays = query.value("shoolDays");
        course.major = query.value("major");
        course.classID = query.value("classID");
        course.rate_attendance = query.value("rate_attendance");
        course.rate_homework = query.value("rate_homework");
        course.rate_experiment = query.value("rate_experiment");
        course.attendance_reduce_fractions = query.value("attendance_reduce_fractions");
        course.lessonTime = query.value("lessonTime");
        course.schoolYears = query.value("schoolYears");
    }

    return course;
}

bool SQLData::updateSQl(const FinalSheet::StudentData& student,int classid)
{//更新
    QSqlQuery query(db);

    // 先尝试更新记录
    query.prepare(
        "UPDATE students SET "
        "studentName = :studentName, "
        "attendance = :attendance, "
        "attendanceScore = :attendanceScore, "
        "homework = :homework, "
        "sub_homework = :sub_homework, "
        "experiment = :experiment, "
        "sub_experiment = :sub_experiment, "
        "totalScore = :totalScore, "
        "remark = :remark "
        "WHERE studentID = :studentID OR studentName = :studentName");

    query.bindValue(":studentID", student.studentID);
    query.bindValue(":studentName", student.studentName);
    query.bindValue(":attendance", student.attendance);
    query.bindValue(":attendanceScore", student.attendanceScore);
    query.bindValue(":homework", student.homework);
    query.bindValue(":sub_homework", vectorToJson(student.sub_homework));
    query.bindValue(":experiment", student.experiment);
    query.bindValue(":sub_experiment", vectorToJson(student.sub_experiment));
    query.bindValue(":totalScore", student.totalScore);
    query.bindValue(":remark", student.remark);

    if (query.exec()) {
        if (query.numRowsAffected() == 0) { // 没有更新任何记录
            return insertStudentData(student, classid); // 插入新记录
        }
        return true; // 更新成功
    } else {
        qDebug() << "Update failed: " << query.lastError();
        return false;
    }
}




QMap<int,QVector<FinalSheet::StudentData>> SQLData::readStudentData()
{
    QMap<int,QVector<FinalSheet::StudentData>> res;
    QVector<FinalSheet::StudentData> student1DataList;
    QVector<FinalSheet::StudentData> student2DataList;

    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return res;
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
        if(query.value("classid").toInt() == 1)
        {
            student1DataList.append(student);
        }
        else if(query.value("classid").toInt() == 2)
        {
            student2DataList.append(student);
        }
    }
    res.insert(1,student1DataList);
    res.insert(2,student2DataList);

    qDebug() << "Number of class1 students read:" << student1DataList.size();
    qDebug() << "Number of class2 students read:" << student2DataList.size();
    return res;
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

void SQLData::CreateStudentsTable()
{//创建 学生 信息 表
    QString createTableQuery = R"(
    CREATE TABLE IF NOT EXISTS `students` (
        `studentID` varchar(255) NOT NULL,
        `studentName` text,
        `attendance` int DEFAULT NULL,
        `attendanceScore` double DEFAULT NULL,
        `homework` double DEFAULT NULL,
        `sub_homework` text,
        `experiment` double DEFAULT NULL,
        `sub_experiment` text,
        `totalScore` double DEFAULT NULL,
        `remark` text,
        `classid` int,
        PRIMARY KEY (`studentID`)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
    )";

    if(executeUpdate(createTableQuery))
    {
        qDebug()<<"建表成功";
    }
}

void SQLData::CreateCourseTable() {
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS CourseData (
            teacher_name VARCHAR(255),
            shoolDays VARCHAR(255),
            major VARCHAR(255),
            classID VARCHAR(255),
            rate_attendance DOUBLE,
            rate_homework DOUBLE,
            rate_experiment DOUBLE,
            attendance_reduce_fractions DOUBLE,
            lessonTime VARCHAR(255),
            schoolYears VARCHAR(255)
        )
    )";

    if (executeUpdate(createTableQuery)) {
        qDebug() << "建表成功";
    } else {
        qDebug() << "建表失败";
    }
}



void SQLData::disconnect() {
    if (db.isOpen()) {
        db.close();
    }
    QSqlDatabase::removeDatabase(connectionName);
}
