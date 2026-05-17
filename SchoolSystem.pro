QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = SchoolSystem
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    logindialog.cpp \
    studentdashboard.cpp \
    profdashboard.cpp \
    tadashboard.cpp \
    admindashboard.cpp \
    schoolsystem.cpp

HEADERS += \
    mainwindow.h \
    logindialog.h \
    studentdashboard.h \
    profdashboard.h \
    tadashboard.h \
    admindashboard.h \
    schoolsystem.h \
    models.h
