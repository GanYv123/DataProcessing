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

private:
    QComboBox *comboBox_selectYear;
    QPushButton *pb_confirm;
    QPushButton *pb_cancel;

    QString select_data;
};

#endif // CUSTOMDIALOG_H
