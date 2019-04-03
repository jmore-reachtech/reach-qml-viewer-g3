#include <QDebug>

#include "sound.h"
#include "beeper.h"

Beeper::Beeper(QObject *parent) : QObject(parent)
    , m_playbackHandle(nullptr), m_wavePtr(nullptr)
{
    /* Open sound card */
    int err;
    if ((err = snd_pcm_open(&m_playbackHandle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
    {
        qDebug("Can't open sound card default: %s\n", snd_strerror(err));
    } else {
        qDebug() << "Sound card is open";
    }
}

Beeper::~Beeper()
{
    qDebug() << "Closing sound card";
    snd_pcm_close(m_playbackHandle);

    if (m_wavePtr != nullptr) {
        free(m_wavePtr);
    }
}

QString Beeper::soundFile()
{
    return "";
}

void Beeper::setSoundFile(const QString &path)
{
    FILE *file = nullptr;
    struct wav_header header;
    int ret;

    qDebug() << "Opening sound file: " << path;

    memset(&header, 0, sizeof(struct wav_header));

    file = fopen(path.toUtf8().data(), "r");
    if (file == nullptr) {
        qDebug() << "Failed to open wav file: ";
        return;
    }

    if (m_wavePtr != nullptr) {
        free(m_wavePtr);
    }

    //qDebug() << "Reading wav header";

    ret = fread(&header, 1, sizeof(wav_header),file);
    /*
    qDebug() << "Header read: " << ret << endl;

    qDebug() << "Riff: " << (char)header.riff[0] << (char)header.riff[1]
             << (char)header.riff[2] << (char)header.riff[3];
    qDebug() << "Chunk Size: " << header.chunk_size;
    qDebug() << "Wave: " << (char)header.wave[0] << (char)header.wave[1]
             << (char)header.wave[2] << (char)header.wave[3] << endl;

    qDebug() << "Format: " << (char)header.fmt[0] << (char)header.fmt[1] << (char)header.fmt[2];
    qDebug() << "Format Size: " << header.chunk_1_size;
    qDebug() << "Format Type: " << header.format_type;
    qDebug() << "Channels: " << header.channels;
    qDebug() << "Sample per sec: " << header.sample_per_sec;
    qDebug() << "Avg bytes per sec: " << header.bytes_per_sec;
    qDebug() << "Block Align: " << header.block_align;
    qDebug() << "Bits Per Sample: " << header.bits_per_sample;
    qDebug() << "Data Chunk: " << (char)header.data[0] << (char)header.data[1]
             << (char)header.data[2] << (char)header.data[3];
    qDebug() << "Data Size: " << header.data_size << endl;
    */
    m_waveFrames = (header.data_size * 8) / header.bits_per_sample;
   //qDebug() << "Frames: " << m_waveFrames << endl;

    /* We only support a Signed 16 bit little endian, rate 44100 Hz, Mono */
    if ((header.sample_per_sec != HZ_44100) && (header.bits_per_sample != BITS_PER_SAMPLE_16)
            && (header.channels != 2)) {
        qDebug() << "Invalid wav file format, required format is: "
                    "Signed 16 bit Little Endian, Rate 44100 Hz, Mono";
        return;
    }

    m_wavePtr = (uint8_t*)malloc(header.data_size);
    ret = fread(m_wavePtr, 1, header.data_size,file);
    if (ret != (int)header.data_size) {
        qDebug() << "Wav data read failed: expected " << header.data_size << " read " << ret;
    }

    /* Set the audio card's hardware parameters (sample rate, bit resolution, etc) */
    if ((ret = snd_pcm_set_params(m_playbackHandle,
                                  SND_PCM_FORMAT_S16,
                                  SND_PCM_ACCESS_RW_INTERLEAVED,
                                  CHAN_1,
                                  header.sample_per_sec,
                                  1,
                                  50000)) < 0) {
        qDebug("Can't set sound parameters: %s \n", snd_strerror(ret));
        return;
    }

    /* Prepare for first write */
    if ((ret = snd_pcm_prepare(m_playbackHandle)) < 0) {
        qDebug("Error preparing sound card: %s \n", snd_strerror(ret));
        return;
    }

    fclose(file);

    emit soundFileChanged();
}

void Beeper::beep()
{
    snd_pcm_sframes_t   frames  = 0;
    int                 ret     = 0;

    if (m_wavePtr == nullptr) {
        return;
    }

    frames = snd_pcm_writei(m_playbackHandle, m_wavePtr, m_waveFrames);
    if (frames < 0) {
        qDebug("Error playing wave: %s \n", snd_strerror(frames));
        frames = snd_pcm_recover(m_playbackHandle, frames, 0);
    }

    /* Wait for playback to completely finish */
    snd_pcm_drain(m_playbackHandle);

    /* Prepare for next write */
    if ((ret = snd_pcm_prepare(m_playbackHandle)) < 0) {
        qDebug("Error preparing sound card: %s \n", snd_strerror(ret));
    }
}
