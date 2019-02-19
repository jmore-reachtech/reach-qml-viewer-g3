#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QObject>
#include <QQuickView>
#include <QStringList>

class MainviewController : public QQuickView
{
public:
    MainviewController();

public slots:
    void updateView(QStringList msg);

private:

};

#endif // MAINVIEW_H
