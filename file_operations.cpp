#include "file_operations.h"
#include "QVBoxLayout"
#include "QDebug"

File_Operations::File_Operations(QWidget *parent) : QWidget(parent)
{//构造函数
    lineEdit = new QLineEdit;
    lineEdit->setEnabled(false);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(lineEdit);
    this->setAcceptDrops(true);
}

File_Operations::~File_Operations()
{

}

void File_Operations::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction(); //事件数据中存在路径，方向事件
    }
    else
    {
        event->ignore();
    }

}

void File_Operations::dragMoveEvent(QDragMoveEvent *event)
{
}

void File_Operations::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if(mimeData->hasUrls())
    {
        QList<QUrl> urls = mimeData->urls();
        QString fileName = urls.at(0).toLocalFile();
        lineEdit->setText(fileName);
    }
}
