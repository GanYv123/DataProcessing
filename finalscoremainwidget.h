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
signals:
    void destroyedWithSignal(); //界面关闭时触发的信号

public:
    explicit FinalScoreMainWidget(QWidget *parent = nullptr);
    ~FinalScoreMainWidget();

private:
    Ui::FinalScoreMainWidget *ui;

protected:
    void closeEvent(QCloseEvent *event) override;

};

#endif // FINALSCOREMAINWIDGET_H
