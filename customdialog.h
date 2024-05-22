#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>

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

#endif // CUSTOMDIALOG_H
