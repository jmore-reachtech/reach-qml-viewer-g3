#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <QObject>
#include <QFile>

#define BACKLIGHT_CTRL          "/sys/class/backlight/backlight_lcd/brightness"
#define BACKLIGHT_CTRL_INVALID  "/dev/null"

class Backlight : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY backlightChanged)

public:
    explicit Backlight(QObject *parent = nullptr);
    ~Backlight();

    int brightness(void);
    void setBrightness(int brightness);

signals:
    void backlightChanged(void);

public slots:

private:
    QFile m_brightness;
};

#endif // BACKLIGHT_H
