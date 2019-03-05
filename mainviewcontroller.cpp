#include <QDebug>
#include <QQuickItem>

#include "mainviewcontroller.h"
#include "common.h"

MainviewController::MainviewController() :m_settings(APP_SETTINGS_PATH, QSettings::NativeFormat)
{
    m_settings.beginGroup("Qml");
    this->load(QUrl::fromLocalFile(m_settings.value("main_view").value<QString>()));
    m_settings.endGroup();
}

void MainviewController::updateView(QStringList msg)
{
    qDebug() << "UpdateView: " << msg;
    QStringList props = msg[0].split(".");

    if (props.length() != 2) {
        qDebug() << "Invalid prop format, expect prop.value " << msg[0];
        return;
    }

    /* We expect the root object to be a ApplicationWindow element */
    QObject *rootObject = this->rootObjects().first();
    QQuickItem *item = rootObject->findChild<QQuickItem *>(props[0]);

    if (item == nullptr) {
        qDebug() << "View does not contain: " << props[0];
        return;
    }

    bool found = item->setProperty(props[1].toUtf8(), msg[1].toUtf8());

    if (!found) {
         qDebug() << "No property on objectName:" << (msg[1]);
    }
}
