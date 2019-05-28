#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>

#define SYSTEM_RELEASE_FILE "/etc/reach-release"

class System : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString version READ version)

public:
    explicit System(QObject *parent = nullptr);

    QString version(void);

signals:

public slots:
    QString execute(QString cmd);
    QString execute(QString cmd, QStringList args);
    bool executeUpgrade(QStringList args);

private:
    QString m_version;
};

#endif // SYSTEM_H
