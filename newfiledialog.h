#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QVariant>
#include "finalsheet.h"
#include <QListWidgetItem>
#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

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

signals:
    void sendCourseData(const FinalSheet::CourseData courseData) const;

private slots:
    void on_bt_addTeacher_clicked();

    void on_bt_check_clicked();

    void on_bt_ok_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_horizontalSlider_valueChanged(int value);

    //更新详情信息的界面
    void updateStudentInfo(const QString &name, const QString &studentID, const QString &major, const QString &enrollmentDate);

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_2_itemClicked(QListWidgetItem *item);

    void handle_sendFilePath(QString filePath);

private:
    Ui::NewFileDialog *ui;
    QButtonGroup *buttonGroup;
    CourseData courseData;
    //文件的路径
    QString filePath_stuInfors = "NullPath";

};

/**
 * @brief The DragDropLabel class
 * 自定义拖拽 导入文件的 label 双击 打开文件的 label
 */
class DragDropLabel : public QLabel
{
    Q_OBJECT

public:
    explicit DragDropLabel(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event);

private:
    void updateLabelText();
public slots:
    void double_clicked_label();
signals:
    void sendFilePath(QString filePath);

};

#endif // NEWFILEDIALOG_H
