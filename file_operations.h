#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <QWidget>
#include <QDebug>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QLineEdit>

class File_Operations : public QWidget
{
    Q_OBJECT
public:
    explicit File_Operations(QWidget* parent = nullptr);
    ~File_Operations();
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;


    QLineEdit* lineEdit;
};

#endif // FILE_OPERATIONS_H
