#include <QApplication>
#include <QSettings>
#include <QQmlContext>
#include <QDebug>

#include "common.h"
#include "mainviewcontroller.h"
#include "serialcontroller.h"
#include "translator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainviewController view;
    SerialController sc;
    Translator tr;

    QObject::connect(&sc, &SerialController::messageAvailable, &view, &MainviewController::updateView);

    tr.load();
    sc.setTranslator(tr);

    view.setSource(QUrl::fromLocalFile(APP_MAIN_VIEW));
    view.rootContext()->setContextProperty("serial", &sc);
    view.show();

    return a.exec();
}
