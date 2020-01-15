#include <QNetworkInterface>
#include <QHostAddress>

#include "network.h"

Network::Network(QObject *parent) : QObject(parent)
{

}

QString Network::ipAddress()
{
    QNetworkInterface iface = QNetworkInterface::interfaceFromName("eth0");

    qDebug() << "[NET] Up";

    if (!iface.isValid()) {
        return "127.0.0.1";
    }

    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
        {
            qDebug() << address.toString();
            return address.toString();
        }
}


    return "127.0.0.1";
}

void Network::setIpAddress(const QString &ip)
{
    QHostAddress addr(ip);

    qDebug() << "Setting ip address: " << addr;

    emit ipAddressChanged();
}
