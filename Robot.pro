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
    groundpreviewwidget.cpp \
    generateground.cpp \
    groundeditor.cpp \
    generategroundbase.cpp \
    barrackseditor.cpp \
    barrackseditorbase.cpp

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
    groundpreviewwidget.h \
    generateground.h \
    groundeditor.h \
    generategroundbase.h \
    barrackseditor.h \
    barrackseditorbase.h
