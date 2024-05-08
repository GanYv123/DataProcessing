#include "customdialog.h"
#include "QComboBox"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "QPushButton"
#include "QDebug"


CustomDialog::CustomDialog(QWidget *parent) : QDialog(parent)
{
    QFont font = this->font();
    font.setBold(true);
    this->setFont(font);

    setWindowTitle("填写信息");

    comboBox_selectYear = new QComboBox(this);
    pb_cancel = new QPushButton("取消",this);
    pb_confirm = new QPushButton("确认",this);

    for (int year = 2000; year <= 2050; ++year) {
        comboBox_selectYear->addItem(QString::number(year) + " - "+ QString::number(year+1) +" 学年第 1 学期");
        comboBox_selectYear->addItem(QString::number(year) + " - "+ QString::number(year+1) +" 学年第 2 学期");
    }
    this->comboBox_selectYear->setCurrentIndex(23*2);
    QVBoxLayout *vb_layout = new QVBoxLayout(this);
    QHBoxLayout *hb_layout = new QHBoxLayout();


    hb_layout->addWidget(pb_cancel);
    hb_layout->addWidget(pb_confirm);
    vb_layout->addWidget(comboBox_selectYear);
    vb_layout->addLayout(hb_layout);
    this->setLayout(vb_layout);

    this->setWindowFlags(Qt::FramelessWindowHint); // 设置无边框

    connect(pb_cancel,&QPushButton::clicked,this,&QDialog::close);
    connect(pb_confirm,&QPushButton::clicked,this,[this](){
        select_data = this->comboBox_selectYear->currentText();
        qInfo()<<"select data : "+select_data;
        this->accept();
    });

}

void CustomDialog::show_choose_schoolYears()
{
    this->show();
    this->exec();
}

QString CustomDialog::get_select_data()
{
    return this->select_data;
}

CustomDialog_chooseClassID::CustomDialog_chooseClassID(QString classID1, QString classID2, QWidget *parent):
    m_classID1(classID1),m_classID2(classID2),QDialog(parent)
{
    QFont font = this->font();
    font.setBold(true);
    this->setFont(font);

    setWindowTitle("选择班级");
    comboBox_selectClassID = new QComboBox(this);
    pb_cancel = new QPushButton("取消",this);
    pb_confirm = new QPushButton("确认",this);

    comboBox_selectClassID->addItem(classID1);
    comboBox_selectClassID->addItem(QString(classID2));

    QVBoxLayout *vb_layout = new QVBoxLayout(this);
    QHBoxLayout *hb_layout = new QHBoxLayout();

    hb_layout->addWidget(pb_cancel);
    hb_layout->addWidget(pb_confirm);
    vb_layout->addWidget(comboBox_selectClassID);
    vb_layout->addLayout(hb_layout);
    this->setLayout(vb_layout);
    this->setWindowFlags(Qt::FramelessWindowHint); // 设置无边框

    connect(pb_cancel,&QPushButton::clicked,this,&QDialog::close);
    connect(pb_confirm,&QPushButton::clicked,this,[this](){
        currentSelectData = this->comboBox_selectClassID->currentText();
        qInfo()<<"select data : "+currentSelectData;
        this->accept();
    });

}

void CustomDialog_chooseClassID::show_chooseClassID()
{
    this->show();
    this->exec();
}

QString CustomDialog_chooseClassID::get_select_data()
{
    return currentSelectData;
}
