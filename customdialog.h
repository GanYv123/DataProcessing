#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QDesktopServices>
#include <QUrl>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "finalsheet.h"
#include <QStyleOptionSlider>
#include <QSlider>

class QComboBox;

class CustomDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CustomDialog(QWidget *parent = nullptr);

    void show_choose_schoolYears();
    QString get_select_data();

protected:
    QComboBox *comboBox_selectYear;
    QPushButton *pb_confirm;
    QPushButton *pb_cancel;

    QString select_data = "";
};

class CustomDialog_chooseClassID : public QDialog
{
    Q_OBJECT
public:

    explicit CustomDialog_chooseClassID(QString classID1 = "null",QString classID2 = "null",QWidget *parent = nullptr);
    void show_chooseClassID();
    QString get_select_data();

protected:
    QComboBox *comboBox_selectClassID;
    QPushButton *pb_confirm;
    QPushButton *pb_cancel;

    QString m_classID1, m_classID2;
    QString currentSelectData = "Not find";

};

class StudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StudentDialog(QWidget *parent = nullptr);
    ~StudentDialog();

    QString getStudentName() const;
    QString getStudentID() const;
    QString getClassName() const;
    void validateAndAccept();

private:
    QLineEdit *lineEdit_name;
    QLineEdit *lineEdit_id;
    QPushButton *pb_confirm;
    QPushButton *pb_cancel;
    int classNum;
    QRadioButton* rb_class1;
    QRadioButton* rb_class2;
};

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

private slots:
    void openWebsite();

private:
    QLabel *titleLabel;
    QLabel *versionLabel;
    QLabel *websiteLabel;
    QPushButton *okButton;
};
class DeleteStudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteStudentDialog(QWidget *parent = nullptr);

    void addStudentToList(const QString &id, const QString &name, int classNumber);
    int getSelectedClass() const;
    int getSelectedRow() const;
    void clearStudentList(); // 添加清除列表的方法

signals:
    void classSelectionChanged(int classNumber);

private slots:
    void handleClassSelection();

private:
    QListWidget *studentListWidget;
    QRadioButton *rb_class1;
    QRadioButton *rb_class2;
    QPushButton *deleteButton;
};


class connectSQLDialog : public QDialog
{
    Q_OBJECT

public:
    explicit connectSQLDialog(QWidget *parent = nullptr);


    QLineEdit *hostLineEdit;
    QLineEdit *portLineEdit;
    QLineEdit *userLineEdit;
    QLineEdit *passwordLineEdit;
    QLineEdit *dbNameLineEdit;

    QPushButton *testConnectionButton;
    QPushButton *connectButton;
    QPushButton *cancelButton;


private slots:
    void testConnection();
    void connectToDatabase();

private:
    QString m_host;
    int m_port;
    QString m_user;
    QString m_password;
    QString m_dbName;
    QString connectionName;

};

class CustomSlider : public QWidget
{
    Q_OBJECT
public:
    explicit CustomSlider(QWidget *parent = nullptr);
    // 其他成员和方法
    QSlider* createSliderWithStyle(QWidget *parent);
};

#endif // CUSTOMDIALOG_H
