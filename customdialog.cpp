#include "customdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include "QComboBox"
#include "QPushButton"
#include "QDebug"
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QRadioButton>
#include <QMessageBox>

#include <QDesktopServices>
#include <QUrl>

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
    this->comboBox_selectYear->setCurrentIndex(24*2);
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
StudentDialog::StudentDialog(QWidget *parent) : QDialog(parent)
{
    QFont font = this->font();
    font.setBold(true);
    this->setFont(font);

    setWindowTitle("添加学生");

    QLabel *label_name = new QLabel("姓名:", this);
    QLabel *label_id = new QLabel("学号:", this);

    lineEdit_name = new QLineEdit(this);
    lineEdit_id = new QLineEdit(this);

    pb_cancel = new QPushButton("取消", this);
    pb_confirm = new QPushButton("确认", this);

    QVBoxLayout *vb_layout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    formLayout->addRow(label_name, lineEdit_name);
    formLayout->addRow(label_id, lineEdit_id);

    // 创建两个互斥的 QRadioButton，并设置默认选中为班级1
    rb_class1 = new QRadioButton("1班", this);
    rb_class2 = new QRadioButton("2班", this);

    rb_class1->setChecked(true); // 默认选择 1 班

    QHBoxLayout *classLayout = new QHBoxLayout();
    classLayout->addWidget(rb_class1);
    classLayout->addWidget(rb_class2);

    formLayout->addRow("班级:", classLayout);

    vb_layout->addLayout(formLayout);

    QHBoxLayout *hb_layout = new QHBoxLayout();
    hb_layout->addWidget(pb_cancel);
    hb_layout->addWidget(pb_confirm);

    vb_layout->addLayout(hb_layout);

    this->setLayout(vb_layout);

    // 添加正则表达式验证器来验证学号和姓名的格式
    QRegularExpression regExp_id("^[0-9]{12}$"); // 学号要求 12 位数字
    QRegularExpressionValidator *validator_id = new QRegularExpressionValidator(regExp_id, this);
    lineEdit_id->setValidator(validator_id);

    QRegularExpression regExp_name("^[\u4e00-\u9fa5]+$"); // 中文名字
    QRegularExpressionValidator *validator_name = new QRegularExpressionValidator(regExp_name, this);
    lineEdit_name->setValidator(validator_name);

    pb_confirm->setFocus();

    // 连接按钮的信号和槽
    connect(pb_cancel, &QPushButton::clicked, this, &QDialog::close);
    connect(pb_confirm, &QPushButton::clicked, this, &StudentDialog::validateAndAccept);
}



void StudentDialog::validateAndAccept()
{
    QString name = lineEdit_name->text();
    QString id = lineEdit_id->text();
    classNum = rb_class1->isChecked() ? 1 : 2; // 获取选择的班级

    if (name.isEmpty() || id.isEmpty()) {
        QMessageBox::warning(this, "警告", "姓名和学号不能为空！");
        return;
    }

    if (classNum) {
        qDebug() << "1班学生" << name << " " << id;
    } else {
        qDebug() << "2班学生" << name << " " << id;
    }

    accept(); // 如果验证通过，接受对话框
}

StudentDialog::~StudentDialog()
{
}

QString StudentDialog::getStudentName() const
{
    return lineEdit_name->text();
}

QString StudentDialog::getStudentID() const
{
    return lineEdit_id->text();
}

QString StudentDialog::getClassName() const
{
    return QString::number(classNum);
}

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("关于软件");

    titleLabel = new QLabel("软件信息", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    versionLabel = new QLabel("版本号：1.0.0", this);
    versionLabel->setAlignment(Qt::AlignCenter);

    websiteLabel =
        new QLabel("<a style = 'color: red' href=\"https://github.com/GanYv123/DataProcessing/tree/master\">仓库地址</a>", this);
    websiteLabel->setOpenExternalLinks(true);
    websiteLabel->setAlignment(Qt::AlignCenter);
    connect(websiteLabel, SIGNAL(linkActivated(QString)), this, SLOT(openWebsite()));

    okButton = new QPushButton("确定", this);
    connect(okButton, &QPushButton::clicked, this, &AboutDialog::accept);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(versionLabel);
    layout->addWidget(websiteLabel);
    layout->addWidget(okButton);
    setLayout(layout);
    this->resize(100,80);
}

AboutDialog::~AboutDialog()
{
}

void AboutDialog::openWebsite()
{
    QDesktopServices::openUrl(QUrl("https://github.com/GanYv123/DataProcessing/tree/master"));
}

//--------------------------------------------------------------------------------
DeleteStudentDialog::DeleteStudentDialog(QWidget *parent)
    : QDialog(parent),
    studentListWidget(new QListWidget(this)),
    rb_class1(new QRadioButton("1班", this)),
    rb_class2(new QRadioButton("2班", this)),
    deleteButton(new QPushButton("删除", this))
{
    rb_class1->setChecked(true); // 默认选择1班

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *classLayout = new QHBoxLayout();
    classLayout->addWidget(rb_class1);
    classLayout->addWidget(rb_class2);
    mainLayout->addLayout(classLayout);
    mainLayout->addWidget(studentListWidget);
    mainLayout->addWidget(deleteButton);

    setLayout(mainLayout);

    connect(rb_class1, &QRadioButton::toggled, this, &DeleteStudentDialog::handleClassSelection);
    connect(rb_class2, &QRadioButton::toggled, this, &DeleteStudentDialog::handleClassSelection);
    connect(deleteButton, &QPushButton::clicked, this, &QDialog::accept);
}

void DeleteStudentDialog::addStudentToList(const QString &id, const QString &name, int classNumber)
{
    QListWidgetItem *item = new QListWidgetItem(id + " - " + name, studentListWidget);
    item->setData(Qt::UserRole, classNumber);
}

int DeleteStudentDialog::getSelectedClass() const
{
    if (rb_class1->isChecked())
        return 1;
    else if (rb_class2->isChecked())
        return 2;
    else
        return -1;
}

int DeleteStudentDialog::getSelectedRow() const
{
    QListWidgetItem *item = studentListWidget->currentItem();
    if (item)
        return studentListWidget->row(item);
    else
        return -1;
}

void DeleteStudentDialog::handleClassSelection()
{
    int selectedClass = getSelectedClass();
    emit classSelectionChanged(selectedClass);
}

void DeleteStudentDialog::clearStudentList()
{
    studentListWidget->clear();
}
