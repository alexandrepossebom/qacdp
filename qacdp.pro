# -------------------------------------------------
# Project created by QtCreator 2009-09-01T09:54:30
# -------------------------------------------------
QT += network \
    xml \
    webkit
TARGET = qacdp
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    acdp.cpp \
    login.cpp \
    getcalendartips.cpp \
    calendarthread.cpp \
    overtimedialog.cpp
HEADERS += mainwindow.h \
    acdp.h \
    login.h \
    getcalendartips.h \
    calendarthread.h \
    overtimedialog.h
FORMS += mainwindow.ui \
    login.ui \
    overtimedialog.ui
