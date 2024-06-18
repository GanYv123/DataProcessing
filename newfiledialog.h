#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QVariant>

namespace Ui {
class NewFileDialog;
}

class NewFileDialog : public QDialog
{
    Q_OBJECT
protected:
    void mySetStyleSheet();
    void connectSignalANDSlot();

public:
    explicit NewFileDialog(QWidget *parent = nullptr);
    ~NewFileDialog();

    struct CourseData{
        QVariant teacher_name = QVariant();
        QVariant shoolDays = QVariant();
        QVariant major = QVariant();
        QVariant classID = QVariant();
        QVariant classType = QVariant();
        QVariant number_of_classes = QVariant();
        QVariant className = QVariant();
        QVariant rate_attendance = QVariant();
        QVariant rate_homework = QVariant();
        QVariant rate_experiment = QVariant();
        QVariant attendance_reduce_fractions = QVariant(); //考勤扣分
        QVariant lessonTime = QVariant();//课时
        QVariant credit = QVariant(); //学分
        QVariant schoolYears = QVariant();
    };

    CourseData getCourseData() const;
    void setCourseData(const CourseData &newCourseData);

private slots:
    void on_bt_addTeacher_clicked();

    void on_bt_check_clicked();

    void on_bt_ok_clicked();

private:
    Ui::NewFileDialog *ui;
    QButtonGroup *buttonGroup;
    CourseData courseData;

};

#endif // NEWFILEDIALOG_H
