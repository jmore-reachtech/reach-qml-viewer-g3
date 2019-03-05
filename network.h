#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>

class Network : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ipAddress READ ipAddress WRITE setIpAddress NOTIFY ipAddressChanged)

public:
    explicit Network(QObject *parent = nullptr);

    QString ipAddress(void);
    void setIpAddress(const QString &ip);

signals:
    void ipAddressChanged();

};

#endif // NETWORK_H
