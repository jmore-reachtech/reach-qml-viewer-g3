#include <QDebug>

#include "gpiopin.h"
#include "gpiocontroller.h"

GpioPin::GpioPin(QObject *parent) : QObject(parent)
  , m_num(-1), m_dir("in"), m_val(-1), m_pinController(GpioController::instance())
{
    connect(&m_poll, &QTimer::timeout, this, &GpioPin::onPinPoll );
}

GpioPin::~GpioPin()
{

}

QString GpioPin::direction()
{
    m_dir = m_pinController.direction(m_num);

    return m_dir;
}

void GpioPin::setDirection(const QString &dir)
{
    m_dir = dir;
    m_pinController.setDirection(m_num, dir);

    emit pinDirectionChanged(dir);
}

int GpioPin::value()
{
    m_val = m_pinController.pin(m_num);

    return m_val;
}

void GpioPin::setValue(int val)
{
    if (m_dir.compare("in") == 0) {
        qDebug() << "cannot set input pin" << m_num << " value";
        return;
    }

    m_val = val;
    m_pinController.setPin(m_num, val);

    emit pinValueChanged(val);
}

void GpioPin::onPinPoll()
{
    auto val = m_pinController.pin(m_num);

    if (val == m_val) {
        /* no change */
        return;
    }

    m_val = val;

    emit pinValueChanged(val);
}

int GpioPin::number()
{
    return m_num;
}

void GpioPin::setNumber(int val)
{
    if (m_num >= 0) {
        qDebug() << "Pin number " << val << "is invalid";
        return;
    }
    m_num = val;
}

bool GpioPin::poll()
{
    return m_poll.isActive();
}

void GpioPin::setPoll(bool poll)
{    
    if (m_pinController.direction(m_num) == "out") {
        qDebug() << "Cannot poll output pin";
        return;
    }

    m_poll.stop();

    if (poll) {
        m_poll.start(1000);
    }
}
