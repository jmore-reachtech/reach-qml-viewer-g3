#include <QApplication>
#include <QSettings>

#include "system.h"
#include "mainviewcontroller.h"
#include "serialcontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainviewController view;
    SerialController sc;

    QObject::connect(&sc, &SerialController::MessageAvailable, &view, &MainviewController::UpdateView);

    view.setSource(QUrl::fromLocalFile(APP_MAIN_VIEW));
    view.show();

    return a.exec();
}



