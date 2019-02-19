#include <QIODevice>
#include <QTextStream>
#include <QMap>
#include <QDebug>

#include "translator.h"
#include "common.h"

Translator::Translator(QObject *parent): QObject(parent)
  ,m_settings(APP_SETTINGS_PATH, QSettings::NativeFormat)
{
    m_settings.beginGroup("Translate");
    m_translateFile.setFileName(m_settings.value("translate_file").value<QString>());
    m_settings.endGroup();
}

Translator::~Translator()
{
    if (m_translateFile.isOpen()) {
        m_translateFile.close();
    }
}

bool Translator::load()
{
    QTextStream in;
    int num = 1;

    qDebug() << "translate load";

    m_guiMap.clear();
    m_microMap.clear();

    if (!m_translateFile.open(QIODevice::ReadOnly| QIODevice::Text)) {
        qDebug() << "translate file failed to open: ";
        return false;
    }

    qDebug() << "translate file open";

    in.setDevice(&m_translateFile);
    while (!in.atEnd()) {
         QString line = in.readLine();

         if (line.isEmpty() || line.startsWith("#")) {
             qDebug() << "skipping comment line: " << num;
         } else {
             addMapping(line, num);
         }
         num++;
    }

    qDebug() << "Map size: " << m_guiMap.size() << ", " << m_microMap.size();

    return true;
}

QStringList Translator::translateGui(QString msg)
{
    QStringList list = msg.split("=");
    qDebug() << "translate gui msg ";
    return list;
}

QStringList Translator::translateSerial(QString msg)
{
    QStringList split = msg.split("=");

    if (split.length() != 2) {
        qDebug() << "msg in wrong format, missing equals";
        return split;
    }

    qDebug() << "translate gui msg, key = " << split[0];
    if (!m_microMap.contains(split[0])) {
        qDebug() << "invalid key: " << split[0];
        return split;
    }

    QStringList lst;
    lst << m_microMap[split[0]] << split[1];

    return lst;
}

/*
    The translate file has the following format

    O:K,T\n

    O = Origin (G=Gui, M=Micro)
    K = Key
    T = Target

    Example
    M:a100,txtCount.text
*/
bool Translator::addMapping(QString line, int num)
{
    QString origin, key, target, msg;

    /* Check for empty line */
    if (line == "\n" || line == "\r") {
        return false;
    }

    if (line.startsWith("G") || line.startsWith("M")) {
        /* Split the line on the colon */
        QStringList list = line.split(":");
        if (list.length() != 2) {
            qDebug() << "line in wrong format, missing comma:" << num;
            return false;
        }

        /* Split on the comma */
        QStringList targetPart = list[1].split(",");

        if (targetPart.length() != 2){
            qDebug() << "line in wrong format, missing key,value,target " << num;
            return false;
        }

       qDebug() << "origin: " << list[0] << "key: " << targetPart[0]
                << ", target: " << targetPart[1];

       if (list[0] == "M") {
           m_microMap.insert(targetPart[0],targetPart[1]);
       } else {
           m_guiMap.insert(targetPart[0],targetPart[1]);
       }

    } else {
        qDebug() << "Translate line " << num << " start invalid, must be M or G";
        return false;
    }

    return true;
}
