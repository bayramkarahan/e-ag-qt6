#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

#include "../interfaces/IVideoCapture.h"
#include<QPixmap>
#include <QScreen>
#include <QImage>


class ScreenCapture : public IVideoCapture
{

public:

    ScreenCapture();

    ~ScreenCapture() override;


    bool open(const VideoCaptureSettings &settings) override;

    bool start() override;

    void stop() override;

    void close() override;


    bool grab(MediaFrame &frame) override;

    void release(MediaFrame &frame) override;


    void setMasterTime(MasterTime *masterTime) override;


private:

    QScreen *m_screen = nullptr;

    bool m_running = false;

    VideoCaptureSettings m_settings;

    MasterTime *m_masterTime = nullptr;
    //qint64 m_videoPts = 0;
};

#endif
