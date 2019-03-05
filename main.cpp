#include <QApplication>
#include <QSettings>
#include <QQmlContext>
#include <QDebug>

#include "common.h"
#include "mainviewcontroller.h"
#include "serialcontroller.h"
#include "translator.h"
#include "network.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SerialController sc;

    /* Need to register before the MainviewController is instaniated */
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

    return a.exec();
}
