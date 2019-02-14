#ifndef SERIALCONTROLLER_H
#define SERIALCONTROLLER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QIODevice>
#include <QSettings>

class SerialController : public QObject
{
    Q_OBJECT
public:
    explicit SerialController(QObject *parent = nullptr);
    ~SerialController();

signals:
    void MessageAvailable(QByteArray ba);

public slots:

private slots:
    void onSerialReadyRead(void);

private:
    QSerialPort m_port;
    QSettings m_settings;
};

#endif // SERIALCONTROLLER_H
