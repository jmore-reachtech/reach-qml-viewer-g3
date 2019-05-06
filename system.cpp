#include <QFile>
#include <QDebug>

#include "system.h"

System::System(QObject *parent) : QObject(parent)
  ,m_version("")
{

}

QString System::version()
{
    if (!m_version.isEmpty()) {
        return m_version;
    }
    QFile f(SYSTEM_RELEASE_FILE);

    if (!f.exists()) {
        qDebug() << "release file not found";
        return "0.0.0";
    }

    qDebug() << "reading system version";
    f.open(QIODevice::ReadOnly);
    QTextStream in(&f);
    m_version = in.readLine();
    f.close();

    return m_version;
}
