#include <QApplication>
#include <QSettings>
#include <QQmlContext>
#include <QDebug>

#include <signal.h>

#include "common.h"
#include "mainviewcontroller.h"
#include "serialcontroller.h"
#include "translator.h"
#include "network.h"

static void unixSignalHandler(int signum) {
    qDebug("[QML] main.cpp::unixSignalHandler(). signal = %s", strsignal(signum));

    qApp->exit(0);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SerialController sc;

    /* Need to register before the MainviewController is instantiated */
    qmlRegisterType<Network>("net.reachtech", 1, 0, "Network");

    MainviewController mv;

#ifdef USE_TRANSLATIONS
    Translator tr;
    qDebug() << "Use translations";
    tr.load();
    sc.setTranslator(tr);
#else
    qDebug() << "Translations disabled";
#endif

    /* Pass serial messages from the SerialController to the MainviewController */
    QObject::connect(&sc, &SerialController::messageAvailable, &mv, &MainviewController::updateView);

    /* Set a signal handler for a power down or a control-c */
    if (signal(SIGTERM, unixSignalHandler) == SIG_ERR) {
        qDebug() << "[QML] an error occurred while setting a signal terminate handler";
    }
    if (signal(SIGINT, unixSignalHandler) == SIG_ERR) {
        qDebug() << "[QML] an error occurred while setting a signal interrupt handler";
    }

    return a.exec();
}
