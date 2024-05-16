QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    course.cpp \
    customdialog.cpp \
    finalsheet.cpp \
    main.cpp \
    mainwindow.cpp \
    mysettings.cpp \
    operexcel.cpp \
    student.cpp

HEADERS += \
    course.h \
    customdialog.h \
    finalsheet.h \
    mainwindow.h \
    mysettings.h \
    operexcel.h \
    student.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    formwork.qrc \
    icons.qrc

# 针对 Linux 的 RC_ICONS 设置
unix:RC_ICONS = fav.ico

# 包含 QXlsx 库
include($$PWD/QXlsx/QXlsx.pri)

# 在 Linux 上，确保包含路径正确
unix {
    INCLUDEPATH += $$PWD/QXlsx
    INCLUDEPATH += /usr/include/boost  # 在 Linux 上安装 boost 的路径
    LIBS += -lxcb  # 添加 Linux 特定的库
}

# 在 Windows 上，确保包含路径正确
win32 {
    INCLUDEPATH += $$PWD/QXlsx
    INCLUDEPATH += D:/OPLIB/boost_1_85_0
    RC_ICONS = fav.ico  # Windows 的图标设置
}

# 确保在不同平台上使用正确的编译器
win32:QMAKE_CXX = g++
unix:QMAKE_CXX = g++
