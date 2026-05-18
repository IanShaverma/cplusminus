QT += core gui network widgets
TARGET = UDPServerPlot
TEMPLATE = app

SOURCES += main.cpp \
        mainwindow.cpp

HEADERS += mainwindow.h

FORMS += mainwindow.ui

INCLUDEPATH += Путьк QCustomPlot
LIBS += -L -lqcustomplot