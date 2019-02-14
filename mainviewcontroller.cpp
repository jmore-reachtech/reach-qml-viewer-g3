#include <QDebug>

#include "mainviewcontroller.h"

MainviewController::MainviewController()
{

}

void MainviewController::UpdateView(QByteArray ba)
{
    QString msg(ba);

    qDebug() << "UpdateView: " << msg;
}
