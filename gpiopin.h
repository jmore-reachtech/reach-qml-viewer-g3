#ifndef GPIOPIN_H
#define GPIOPIN_H

#include <QObject>
#include <QTimer>

#include "gpiocontroller.h"

class GpioPin : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int number READ number WRITE setNumber)
    Q_PROPERTY(int value READ value NOTIFY pinValueChanged)
    Q_PROPERTY(bool poll READ poll WRITE setPoll NOTIFY pollChanged)
    Q_PROPERTY(QString direction READ direction WRITE setDirection NOTIFY pinDirectionChanged)

public:
    explicit GpioPin(QObject *parent = nullptr);
    ~GpioPin();

    QString direction(void);
    int value(void);
    int number(void);
    void setNumber(int);
    bool poll(void);
    void setPoll(bool);


signals:
    void pinDirectionChanged(const QString&);
    void pinValueChanged(int);
    void pollChanged(void);

public slots:
    void setDirection(const QString&);
    void setValue(int);

private slots:
    void onPinPoll(void);

private:
    int             m_num;
    QString         m_dir;
    int             m_val;
    QTimer          m_poll;
    GpioController& m_pinController;
};

#endif // GPIOPIN_H
