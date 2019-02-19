#include <QtDebug>

#include "serialcontroller.h"
#include "common.h"

SerialController::SerialController(QObject *parent) : QObject(parent)
  ,m_settings(APP_SETTINGS_PATH, QSettings::NativeFormat), m_translator(nullptr)
{
    m_settings.beginGroup("Serial");
    m_port.setPortName(m_settings.value("serial_port").value<QString>());
    m_port.setBaudRate(m_settings.value("serial_baud").value<QSerialPort::BaudRate>()
                     , QSerialPort::AllDirections);
    m_port.setDataBits(m_settings.value("serial_data_bits").value<QSerialPort::DataBits>());
    m_port.setParity(m_settings.value("serial_parity").value<QSerialPort::Parity>());
    m_port.setStopBits(m_settings.value("serial_stop_bits").value<QSerialPort::StopBits>());
    m_port.setFlowControl(QSerialPort::NoFlowControl);
    m_settings.endGroup();

    if (m_port.open(QIODevice::ReadWrite)) {
        qDebug() << "";
        qDebug() << "Serial port " << m_port.portName() << " is open";
        qDebug() << "Baud rate" << m_port.baudRate();
        qDebug() << "Data bits" << m_port.dataBits();
        qDebug() << "Stop bits" << m_port.stopBits();
        qDebug() << "Parity" << m_port.parity();
        qDebug() << "";

        connect(&m_port, &QSerialPort::readyRead, this, &SerialController::onSerialReadyRead );

    } else {
        qDebug() << "Serial port error: Could not open" << m_port.portName() << " : " << m_port.errorString();
    }
}

SerialController::~SerialController()
{
    qDebug() << "closing serial port";
    m_port.close();
}

void SerialController::setTranslator(Translator& t)
{
    qDebug() << "serial set translator";
    m_translator = &t;
}

void SerialController::send(QString msg)
{
    if (m_translator == nullptr) {
        return;
    }

    m_port.write(m_translator->translateGui(msg).join("").toUtf8());
}

void SerialController::onSerialReadyRead()
{
    while (m_port.bytesAvailable() && m_port.canReadLine()) {
        qDebug() << "emit message";
        QByteArray ba = m_port.readLine();

        emit messageAvailable(m_translator->translateSerial(QString(ba)));
    }
}
