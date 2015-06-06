#Enabled the qt modules.
QT += core gui widgets

#Enabled c++ 11 syntax.
CONFIG += c++11

#Win32 configure.
win32
{
    DISTFILES += \
        win.rc
    RC_FILE += win.rc
}

macx
{
    ICON = icon.icns
}

TRANSLATIONS += res/Simplified_Chinese.ts \
                res/Traditional_Chinese.ts \
                res/English.ts \
                res/Japanese.ts

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
    robotmanagewidget.cpp \
    about.cpp \
    gridwidget.cpp \
    robotbase.cpp \
    enemy.cpp \
    enemyaddwidget.cpp \
    enemymanagewidget.cpp \
    enemymanagement.cpp \
    languagemanager.cpp

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
    robotmanagewidget.h \
    about.h \
    gridwidget.h \
    robotbase.h \
    mainpage.h \
    enemy.h \
    enemyaddwidget.h \
    enemymanagewidget.h \
    enemymanagement.h \
    languagemanager.h

RESOURCES += \
    res.qrc
