#-------------------------------------------------
#
# Project created by QtCreator 2019-02-11T08:53:18
#
#-------------------------------------------------

QT       += core quick serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lasound

TARGET = qml-viewer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# If you want to use the translate.conf file for message translations, set USE_TRANSLATIONS
#DEFINES += USE_TRANSLATIONS

CONFIG += c++11

SOURCES += \
    backlight.cpp \
        main.cpp \
    serialcontroller.cpp \
    mainviewcontroller.cpp \
    translator.cpp \
    network.cpp \
    beeper.cpp \
    gpiopin.cpp \
    gpiocontroller.cpp

HEADERS += \
    backlight.h \
    serialcontroller.h \
    mainviewcontroller.h \
    common.h \
    translator.h \
    network.h \
    beeper.h \
    sound.h \
    i2c-dev.h \
    gpiopin.h \
    gpiocontroller.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    settings.conf.example \
    translate.conf.example
