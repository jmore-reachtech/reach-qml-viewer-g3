#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QObject>
#include <QQuickView>
#include <QStringList>
#include <QSettings>

class MainviewController : public QQuickView
{
public:
    MainviewController();

public slots:
    void updateView(QStringList msg);

private:
    QSettings m_settings;

};

#endif // MAINVIEW_H
