#include <QFile>
#include <QProcess>
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

QString System::execute(QString cmd)
{
    QProcess p(this);

    p.start(cmd);

    p.waitForFinished();

    QByteArray data = p.readAllStandardOutput();

    return QString::fromLatin1(data.data());
}

QString System::execute(QString cmd, QStringList args)
{
    QProcess p(this);

    p.start(cmd, args);

    p.waitForFinished();

    QByteArray data = p.readAllStandardOutput();

    return QString::fromLatin1(data.data());
}

bool System::executeUpgrade(QStringList args)
{
    return QProcess::startDetached("/usr/bin/qml-upgrade-helper.sh", args, "/data");

}
