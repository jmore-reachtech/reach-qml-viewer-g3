#ifndef GPIOCONTROLLER_H
#define GPIOCONTROLLER_H

#include <QObject>
#include <QList>

#define GPIO_DEV "/dev/i2c-0"
#define GPIO_I2C_ADDR       0x3E
#define GPIO_INPUT_REG      0x00
#define GPIO_OUT_REG        0x01
#define GPIO_POLARITY_REG   0x02
#define GPIO_CTRL_REG       0x03
#define GPIO_MIN_PIN        0
#define GPIO_MAX_PIN        7
#define GPIO_PIN_HIGH       1
#define GPIO_PIN_LOW        0
#define GPIO_PIN_OUTPUT     GPIO_PIN_LOW
#define GPIO_PIN_INPUT      GPIO_PIN_HIGH

class GpioController : public QObject
{
    Q_OBJECT

public:
    static GpioController& instance(){
        static GpioController gc;
        return gc;
    }
    GpioController(const GpioController&) = delete;
    GpioController(GpioController&&) = delete;
    GpioController& operator=(const GpioController&) = delete;
    GpioController& operator=(GpioController&&) = delete;

    int pin(int pin);
    bool setPin(int pin, int val);
    QString direction(int pin);
    bool setDirection(int pin, QString dir);

signals:

public slots:

private:
    GpioController(QObject *parent = nullptr);
    ~GpioController();

    int m_fd;
};

#endif // GPIOCONTROLLER_H
