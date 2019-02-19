#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QMap>
#include <QObject>
#include <QSettings>
#include <QFile>

class Translator : public QObject
{
    Q_OBJECT

public:
    Translator(QObject *parent = 0);
    ~Translator();

    bool load(void);
    QStringList translateGui(QString msg);
    QStringList translateSerial(QString msg);

private:
    bool addMapping(QString msg, int num);
    QMap<QString,QString> m_guiMap;
    QMap<QString,QString> m_microMap;
    QSettings m_settings;
    QFile m_translateFile;
};

#endif // TRANSLATOR_H
