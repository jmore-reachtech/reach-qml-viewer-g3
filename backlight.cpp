#include <QTextStream>
#include <QDebug>

#include "backlight.h"

Backlight::Backlight(QObject *parent) : QObject(parent)
  ,m_brightness(BACKLIGHT_CTRL)
{
    if (!m_brightness.exists()) {
        qDebug() << "backlight control '" << BACKLIGHT_CTRL << "' does not exist";
        m_brightness.setFileName(BACKLIGHT_CTRL_INVALID);
    }

    m_brightness.open(QIODevice::ReadWrite);
}

Backlight::~Backlight()
{
    if (m_brightness.isOpen()) {
        qDebug() << "closing backlight control";
        m_brightness.close();
    }
}

int Backlight::brightness()
{
    QTextStream in(&m_brightness);
    QString s = in.readLine();

    return s.toInt();;
}

void Backlight::setBrightness(int brightness)
{
    QTextStream out(&m_brightness);
    out << QString::number(brightness).toLatin1() << endl;
}
