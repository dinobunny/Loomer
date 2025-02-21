QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += C:/msgpack/msgpack-c/include
INCLUDEPATH += C:/boost_1_87_0

SOURCES += \
    customwidgetitem.cpp \
    getpath.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    config.h \
    customwidgetitem.h \
    enums.h \
    getpath.h \
    m_pack.h \
    m_pack.h \
    mainwindow.h


FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
