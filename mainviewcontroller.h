#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QObject>
#include <QQuickView>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStringList>
#include <QSettings>

class MainviewController : public QQmlApplicationEngine
{
public:
    MainviewController();

public slots:
    void updateView(QStringList msg);

private:
    QSettings m_settings;

};

#endif // MAINVIEW_H
