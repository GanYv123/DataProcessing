#ifndef FINALSCOREMAINWIDGET_H
#define FINALSCOREMAINWIDGET_H

#include <QMainWindow>
#include <QDebug>

namespace Ui {
class FinalScoreMainWidget;
}

class FinalScoreMainWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit FinalScoreMainWidget(QWidget *parent = nullptr);
    ~FinalScoreMainWidget();

protected:
    void initUI();

public:
signals:
    void destroyedWithSignal(); //界面关闭时触发的信号

private:
    Ui::FinalScoreMainWidget *ui;

protected:
    void closeEvent(QCloseEvent *event) override;

};

#endif // FINALSCOREMAINWIDGET_H
