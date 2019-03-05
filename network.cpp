#include <QNetworkInterface>
#include <QHostAddress>

#include "network.h"

Network::Network(QObject *parent) : QObject(parent)
{

}

QString Network::ipAddress()
{
    QNetworkInterface iface = QNetworkInterface::interfaceFromName("eth0");

    if (!iface.isValid()) {
        return "127.0.0.1";
    }

    foreach (auto ae, iface.addressEntries()) {
        if (ae.ip().protocol() == QAbstractSocket::IPv4Protocol) {
            return ae.ip().toString();
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
