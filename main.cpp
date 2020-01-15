#include <QGuiApplication>
#include <QQmlApplicationEngine>

//#include <QApplication>

#include <QQuickWindow>

#include <QSettings>
#include <QQmlContext>
#include <QDebug>

#include <signal.h>

#include "common.h"
#include "mainviewcontroller.h"
#include "serialcontroller.h"
#include "translator.h"
#include "network.h"
#include "beeper.h"
#include "gpiopin.h"
#include "backlight.h"
#include "system.h"

static void unixSignalHandler(int signum) {
    qDebug("[QML] main.cpp::unixSignalHandler(). signal = %s", strsignal(signum));

    qApp->exit(0);
}

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    QQmlApplicationEngine engine;
    SerialController serialController;

    /* Need to register before the MainviewController is instantiated */
    qmlRegisterType<Network>("net.reachtech", 1, 0, "Network");
    qmlRegisterType<Beeper>("sound.reachtech", 1, 0, "Beeper");
    qmlRegisterType<GpioPin>("gpio.reachtech", 1, 0, "GpioPin");
    qmlRegisterType<Backlight>("backlight.reachtech", 1, 0, "Backlight");
    qmlRegisterType<System>("system.reachtech", 1, 0, "System");

    MainviewController mv;

#ifdef USE_TRANSLATIONS
    Translator tr;
    qDebug() << "Use translations";
    tr.load();
    serialController.setTranslator(tr);
#else
    qDebug() << "Translations disabled";
#endif

    /* Pass serial messages from the SerialController to the MainviewController */
    QObject::connect(&serialController, &SerialController::messageAvailable, &mv, &MainviewController::updateView);

    QObject *topLevel = mv.rootObjects().value(0);

    if(topLevel == nullptr)
    {
        qDebug() << "Strike 2!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    }

    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    if(window == nullptr)
    {
        qDebug() << "Strike 3!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    }

    QObject::connect(window, SIGNAL(submitTextField(QString)), &serialController, SLOT(send(QString)) );

    /* Set a signal handler for a power down or a control-c */
    if (signal(SIGTERM, unixSignalHandler) == SIG_ERR) {
        qDebug() << "[QML] an error occurred while setting a signal terminate handler";
    }
    if (signal(SIGINT, unixSignalHandler) == SIG_ERR) {
        qDebug() << "[QML] an error occurred while setting a signal interrupt handler";
    }

    return a.exec();
}
