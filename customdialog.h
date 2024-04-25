#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include "QDialog"

class QComboBox;
class QPushButton;

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

    QString select_data = "falied to select schoolYear";
};

class CustomDialog_chooseClassID : public QDialog
{
    Q_OBJECT
public:

    explicit CustomDialog_chooseClassID(QString classID1,QString classID2,QWidget *parent = nullptr);
    void show_chooseClassID();
    QString get_select_data();

protected:
    QComboBox *comboBox_selectClassID;
    QPushButton *pb_confirm;
    QPushButton *pb_cancel;


    QString m_classID1 = "Null",m_classID2 = "Null";
    QString currentSelectData = "Not find";

};



#endif // CUSTOMDIALOG_H
