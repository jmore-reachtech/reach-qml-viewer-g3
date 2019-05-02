#include <QDebug>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "i2c-dev.h"
#include <iostream>

#include "gpiocontroller.h"

int GpioController::pin(int pin)
{
    uint32_t reg = 0x0;

    if (pin < GPIO_MIN_PIN || pin > GPIO_MAX_PIN) {
        qDebug() << "Pin " << pin << " is invalid!";
        return -1;
    }

    qDebug() << "Getting pin " << pin;
    reg = i2c_smbus_read_byte_data(m_fd, GPIO_CTRL_REG);

    if ((reg >> pin) & 0x1) {
        qDebug() << "Getting output reg ";
        reg = i2c_smbus_read_byte_data(m_fd, GPIO_OUT_REG);
    } else {
        qDebug() << "Getting input reg ";
        reg = i2c_smbus_read_byte_data(m_fd, GPIO_INPUT_REG);
    }

    qDebug() << "reg val: " << QByteArray::number(reg).toHex();

    return (reg >> pin) & 0x1;
}

bool GpioController::setPin(int pin, int val)
{
    uint32_t reg = 0x0;

    if (pin < GPIO_MIN_PIN || pin > GPIO_MAX_PIN) {
        qDebug() << "Pin " << pin << " is invalid!";
        return false;
    }

    qDebug() << "Setting pin " << pin << " value to " << val;

    reg = i2c_smbus_read_byte_data(m_fd, GPIO_CTRL_REG);

    if (((reg >> pin) & 0x1)) {
        qDebug() << "Pin " << pin << " is an input and cannot be set!";
        return false;
    }

    reg = i2c_smbus_read_byte_data(m_fd, GPIO_OUT_REG);

    qDebug() << "out reg 0x" << QByteArray::number(reg, 16);

    if (val == 0) {
        reg &= ~(0x01 << pin);
    } else {
        reg |= (0x1 << pin);
    }

    qDebug() << "new out reg: " << QByteArray::number(reg, 16);

    auto rv = i2c_smbus_write_byte_data(m_fd, GPIO_OUT_REG, reg);
    if (rv != 0) {
        qDebug() << "i2c dev failed to set direction";
        return false;
    }

    return true;
}

QString GpioController::direction(int pin)
{
    uint32_t reg = 0x0;

    if ((pin < GPIO_MIN_PIN) || (pin > GPIO_MAX_PIN)) {
        return "";
    }

    reg = i2c_smbus_read_byte_data(m_fd, GPIO_CTRL_REG);

    qDebug() << "Getting pin " << pin << " direction " << QByteArray::number(reg).toHex();

    if ((reg >> pin) & 0x1) {
        return "in";
    } else {
        return "out";
    }
}

bool GpioController::setDirection(int pin, QString dir)
{
    uint32_t reg = 0x0;

    if (pin < GPIO_MIN_PIN || pin > GPIO_MAX_PIN) {
        qDebug() << "Pin " << pin << " is invalid!";
        return false;
    }

    reg = i2c_smbus_read_byte_data(m_fd, GPIO_CTRL_REG);

    qDebug() << "ctrl reg 0x" << QByteArray::number(reg, 16);

    if (dir.compare("out") == 0) {
        /* set as output - 0 */
        reg &= ~(0x1 << pin);
    } else if (dir.compare("in") == 0){
        /* set as intput - 1 */
        reg |= (0x1 << pin);
    } else {
        /* invalid direction */
        qDebug() << "Direction '" << dir << "' is invalid";
        return false;
    }

    qDebug() << "writing reg 0x" << QByteArray::number(reg, 16);

    auto rv = i2c_smbus_write_byte_data(m_fd, GPIO_CTRL_REG, reg);
    if (rv != 0) {
        qDebug() << "i2c dev failed to set direction";
        return false;
    }

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
    if (rv != 0) {
        qDebug() << "i2c dev failed to set address";
        return;
    }

    /* set all pins as input, the default */
    rv = i2c_smbus_write_byte_data(m_fd, GPIO_CTRL_REG, 0xff);
    if (rv != 0) {
        qDebug() << "i2c dev failed to set output";
        return;
    }

    /* clear the out reg */
    rv = i2c_smbus_write_byte_data(m_fd, GPIO_OUT_REG, 0x0);
    if (rv != 0) {
        qDebug() << "i2c dev failed to set output";
    }
}

GpioController::~GpioController()
{
    if (m_fd > 0) {
        /* reset GPIO pins to input, clear out reg */
        auto rv = i2c_smbus_write_byte_data(m_fd, GPIO_OUT_REG, 0x0);
        if (rv != 0) {
            qDebug() << "i2c dev failed to set output";
        }

        rv = i2c_smbus_write_byte_data(m_fd, GPIO_CTRL_REG, 0xff);
        if (rv != 0) {
            qDebug() << "i2c dev failed to set control";
        }

        qDebug() << "closing i2c dev";
        close(m_fd);
    }
}
