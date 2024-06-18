#include "finalscoremainwidget.h"
#include "ui_finalscoremainwidget.h"
#include <QCloseEvent>

/**
 * @brief FinalScoreMainWidget::FinalScoreMainWidget
 * 构造函数
 * @param parent
 */
FinalScoreMainWidget::FinalScoreMainWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FinalScoreMainWidget)
{
    ui->setupUi(this);
    connect(ui->ac_back,&QAction::triggered,parent,&QWidget::show);
    initUI();
}

FinalScoreMainWidget::~FinalScoreMainWidget()
{
    delete ui;
}

void FinalScoreMainWidget::initUI()
{
    this->resize(840,600);
}


void FinalScoreMainWidget::closeEvent(QCloseEvent *event)
{
    // 在窗口关闭时执行你的操作
    qDebug() << "MainWindow is closing...";
    emit destroyedWithSignal();
    event->accept();
}

