#-------------------------------------------------
#
# Project created by QtCreator 2024-09-01T12:00:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QFotoPaint6

TEMPLATE = app

CONFIG += c++17

SOURCES += main.cpp\
    mainwindow.cpp \
    imagenes.cpp \
    dialognueva.cpp \
    brillocontraste.cpp \
    suavizados.cpp \
    video.cpp \
    rotaravideo.cpp \
    mediaponderada.cpp \
    acercade.cpp \
    mediadevideo.cpp

HEADERS  += mainwindow.h \
    imagenes.h \
    dialognueva.h \
    brillocontraste.h \
    suavizados.h \
    video.h \
    rotaravideo.h \
    mediaponderada.h \
    acercade.h \
    mediadevideo.h

FORMS    += mainwindow.ui \
    dialognueva.ui \
    brillocontraste.ui \
    suavizados.ui \
    rotaravideo.ui \
    mediaponderada.ui \
    acercade.ui \
    mediadevideo.ui

INCLUDEPATH += "C:\OpenCV\OpenCV4.10.0G\include"

LIBS += -L"C:\OpenCV\OpenCV4.10.0G\lib"\
        -llibopencv_world4100

RESOURCES += recursos.qrc

RC_ICONS = imagenes/icono.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
