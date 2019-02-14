#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QObject>
#include <QQuickView>

class MainviewController : public QQuickView
{
public:
    MainviewController();

public slots:
    void UpdateView(QByteArray ba);

private:

};

#endif // MAINVIEW_H
