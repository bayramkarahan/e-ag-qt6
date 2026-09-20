#ifndef MEDIAFILECAPTURE_H
#define MEDIAFILECAPTURE_H

#include "../interfaces/IVideoCapture.h"
#include "../audio/audioframe.h"
#include<QThread>
#include <QQueue>
#include<QDebug>
#include<QMutexLocker>
#include <QWaitCondition>
#include <QMutex>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}
extern "C"
{
#include <libavdevice/avdevice.h>
}
extern "C" {
#include <libavutil/imgutils.h>
}
extern "C" {
#include <libavutil/pixdesc.h>
}
extern "C" {
#include <libavutil/imgutils.h>
}


class MediaFileCapture : public IVideoCapture
{
public:
    MediaFileCapture();
    ~MediaFileCapture() override;

    qint64 m_globalStartPts = AV_NOPTS_VALUE;
    bool grabNext(MediaFrame &video, AudioFrame &audio, bool &isVideo);
void setLoop(bool enabled);
    bool open(const VideoCaptureSettings &settings) override;
    bool start() override;
    void stop() override;
    void close() override;
    void decodeVideoPacket();
    void decodeAudioPacket();

    bool grab(MediaFrame &frame) override;
    void release(MediaFrame &frame) override;

    void setMasterTime(MasterTime *masterTime) override;

    // Ses için
    bool grabAudio(AudioFrame &frame);

    // Yayınlanacak dosya
    void setFileName(const QString &fileName);
    bool readNextPacket();
    bool restart();

    bool seek(qint64 ms);

    void pause(bool pause);

    bool isPaused() const;

    qint64 duration() const;

    qint64 position() const;
    bool isFinished() const;
    bool hasVideoFrame() const;
    bool hasAudioFrame() const;

    qint64 videoPts() const;
    qint64 audioPts() const;
private:

    bool m_loop = false;
    QString m_fileName;

    bool m_running = false;

    VideoCaptureSettings m_settings;

    MasterTime *m_masterTime = nullptr;

    // FFmpeg
    AVFormatContext *m_format = nullptr;

    AVCodecContext *m_videoCodec = nullptr;
    AVCodecContext *m_audioCodec = nullptr;

    AVPacket *m_packet = nullptr;
    AVFrame  *m_frame  = nullptr;

    int m_videoStream = -1;
    int m_audioStream = -1;

    SwsContext *m_sws = nullptr;
    SwrContext *m_swr = nullptr;
    qint64 m_firstPts = -1;
    QQueue<MediaFrame> m_videoQueue;
    QQueue<AudioFrame> m_audioQueue;
    qint64 m_firstVideoPts = AV_NOPTS_VALUE;
    qint64 m_firstAudioPts = AV_NOPTS_VALUE;

    qint64 m_videoStartUs = 0;
    qint64 m_audioStartUs = 0;
    bool m_paused = false;
    bool m_finished = false;

    static constexpr int MIN_VIDEO = 3;
    static constexpr int MIN_AUDIO = 6;

    QWaitCondition m_notFull;

    mutable QMutex m_videoMutex;
    mutable QMutex m_audioMutex;

    static constexpr int MAX_VIDEO_QUEUE = 30;
    static constexpr int MAX_AUDIO_QUEUE = 120;



    QWaitCondition m_notFullVideo;
    QWaitCondition m_notFullAudio;

    QWaitCondition m_notEmptyVideo;
    QWaitCondition m_notEmptyAudio;

};

#endif
