#include <QDebug>

#include "gpiopin.h"
#include "gpiocontroller.h"

GpioPin::GpioPin(QObject *parent) : QObject(parent)
  , m_num(-1), m_dir("in"), m_val(0)
{
    connect(this, &GpioPin::pinDirectionChanged, this, &GpioPin::onPinDirectionChanged );
    connect(this, &GpioPin::pinValueChanged, this, &GpioPin::onPinValueChanged );
    connect(&m_poll, &QTimer::timeout, this, &GpioPin::onPinPoll );
}

GpioPin::~GpioPin()
{

}

QString GpioPin::direction()
{
    m_dir = GpioController::instance().pin(m_num);

    return m_dir;
}

void GpioPin::setDirection(const QString &dir)
{
    m_dir = dir;
    emit pinDirectionChanged(dir);
}

int GpioPin::value()
{
    m_val = GpioController::instance().pin(m_num);

    return m_val;
}

void GpioPin::setValue(int val)
{
    if (m_val == val) {
        /* nothing changed */
        return;
    }
    m_val = val;
    emit pinValueChanged(val);
}

void GpioPin::onPinDirectionChanged(const QString &dir)
{
    GpioController::instance().setDirection(m_num, dir);
}

void GpioPin::onPinValueChanged(int val)
{
    if (m_dir.compare("in") == 0) {
        qDebug() << "cannot set input pin value";
        return;
    }
    GpioController::instance().setPin(m_num, val);
}

void GpioPin::onPinPoll()
{
    setValue(GpioController::instance().pin(m_num));
}

int GpioPin::number()
{
    return m_num;
}

void GpioPin::setNumber(int val)
{
    if (m_num >= 0) {
        qDebug() << "Cannot change GPIO pin assignment";
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
    m_poll.stop();

    if (poll) {
        m_poll.start(1000);
    }
}
