#include "finalscoremainwidget.h"
#include "ui_finalscoremainwidget.h"
#include <QCloseEvent>

FinalScoreMainWidget::FinalScoreMainWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FinalScoreMainWidget)
{
    ui->setupUi(this);
    connect(ui->ac_back,&QAction::triggered,parent,&QWidget::show);
}

FinalScoreMainWidget::~FinalScoreMainWidget()
{
    delete ui;
}

void FinalScoreMainWidget::closeEvent(QCloseEvent *event)
{
    // 在窗口关闭时执行你的操作
    qDebug() << "MainWindow is closing...";
    emit destroyedWithSignal();
    event->accept();
}

