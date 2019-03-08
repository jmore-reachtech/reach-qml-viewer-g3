#ifndef BEEPER_H
#define BEEPER_H

#include <QObject>

#include <alsa/asoundlib.h>
#include <alsa/control.h>

class Beeper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString soundFile READ soundFile WRITE setSoundFile NOTIFY soundFileChanged)

public:
    explicit Beeper(QObject *parent = nullptr);
    ~Beeper();

    QString soundFile(void);
    void setSoundFile(const QString &path);

signals:
    void soundFileChanged();

public slots:
    void beep(void);

private:
    snd_pcm_t           *m_playbackHandle;
    uint8_t             *m_wavePtr;
    snd_pcm_sframes_t   m_waveFrames;
};

#endif // BEEPER_H
