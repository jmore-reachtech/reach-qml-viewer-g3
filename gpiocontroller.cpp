#include <QDebug>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "i2c-dev.h"

#include "gpiocontroller.h"

int GpioController::pin(int pin)
{
    qDebug() << "Getting pin " << pin << " value " << 0;

    return 0;
}

bool GpioController::setPin(int pin, int val)
{
    qDebug() << "Setting pin " << pin << " value to " << val;

    return true;
}

QString GpioController::direction(int pin)
{
    qDebug() << "Getting pin " << pin << " direction out";

    return "out";
}

bool GpioController::setDirection(int pin, QString dir)
{
    qDebug() << "Setting pin " << pin << " direction to " << dir;

    return true;
}

GpioController::GpioController(QObject *parent): QObject(parent) ,
    m_fd(0)
{
    qDebug() << "GpioController constructor";

    m_fd = open(GPIO_DEV, O_RDWR);
    if (m_fd > 0) {
        qDebug() << "i2c dev open";
    } else {
        qDebug() << "i2c dev failed to open";
        return;
    }

    auto rv = ioctl(m_fd, I2C_SLAVE, GPIO_I2C_ADDR);
    if (rv < 0) {
        qDebug() << "i2c dev failed to set address";
        return;
    }

    /* set all pins as input */
    rv = i2c_smbus_write_byte_data(m_fd, GPIO_CTRL_REG, 0x0);
    if (rv < 0) {
        qDebug() << "i2c dev failed to set output";
        return;
    }
}

GpioController::~GpioController()
{
    if (m_fd > 0) {
        qDebug() << "closing i2c dev";
        close(m_fd);
    }
}
