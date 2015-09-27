#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T09:42:18
#
#-------------------------------------------------
#QMAKE_CXXFLAGS += -std=c++0x
#DEFINES += __GXX_EXPERIMENTAL_CXX0X__

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    test1dialog.cpp \
    test2dialog.cpp \
    getstudentdialog.cpp \
    test3dialog.cpp \
    test4dialog.cpp \
    test5dialog.cpp \
    test6dialog.cpp \
    studentdialog.cpp \
    resultdialog.cpp \
    tabledialog.cpp

HEADERS  += mainwindow.h \
    test1dialog.h \
    test2dialog.h \
    getstudentdialog.h \
    test3dialog.h \
    test4dialog.h \
    test5dialog.h \
    test6dialog.h \
    studentdialog.h \
    resultdialog.h \
    tabledialog.h

FORMS    += mainwindow.ui \
    test1dialog.ui \
    test2dialog.ui \
    getstudentdialog.ui \
    test3dialog.ui \
    test4dialog.ui \
    test5dialog.ui \
    test6dialog.ui \
    studentdialog.ui \
    resultdialog.ui \
    tabledialog.ui

RESOURCES += \
    LTest.qrc

#DISTFILES += \
#    LTest.rc


win32 {

    RC_FILE += LTest.rc
}


