#Enabled the qt modules.
QT += core gui widgets

#Enabled c++ 11 syntax.
CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ground.cpp \
    robot.cpp \
    panel.cpp \
    menubar.cpp \
    robotmanagement.cpp \
    paneldock.cpp \
    robotaddwidget.cpp \
    generateground.cpp \
    pointeditor.cpp \
    groundpreviewer.cpp \
    groundrealtimepreviewer.cpp \
    groundglobal.cpp \
    polygoneditor.cpp \
    robotmanagewidget.cpp

HEADERS += \
    mainwindow.h \
    ground.h \
    robot.h \
    panel.h \
    groundbase.h \
    menubar.h \
    robotmanagement.h \
    paneldock.h \
    robotaddwidget.h \
    generateground.h \
    generategroundbase.h \
    pointeditor.h \
    groundpreviewer.h \
    groundrealtimepreviewer.h \
    groundglobal.h \
    polygoneditor.h \
    robotmanagewidget.h
