#include "screencapture.h"

#include <QGuiApplication>
#include <QDebug>
#include <QImage>


ScreenCapture::ScreenCapture()
{
}


ScreenCapture::~ScreenCapture()
{
    close();
}


bool ScreenCapture::open(const VideoCaptureSettings &settings)
{
    m_settings = settings;


    QList<QScreen*> screens =
            QGuiApplication::screens();


    if(screens.isEmpty())
    {
        qWarning() << "No screen found.";
        return false;
    }


    int index = settings.screenIndex;


    if(index < 0 || index >= screens.size())
        index = 0;


    m_screen = screens.at(index);


    if(!m_screen)
    {
        qWarning() << "Screen pointer invalid.";
        return false;
    }


    qDebug()
            << "Screen opened:"
            << m_screen->name()
            << m_screen->size();


    return true;
}



bool ScreenCapture::start()
{
    if(!m_screen)
        return false;


    m_running = true;

    return true;
}



void ScreenCapture::stop()
{
    m_running = false;
}



void ScreenCapture::close()
{
    stop();

    m_screen = nullptr;
}



void ScreenCapture::setMasterTime(MasterTime *masterTime)
{
    m_masterTime = masterTime;
}



bool ScreenCapture::grab(MediaFrame &frame)
{
    if(!m_running || !m_screen)
        return false;

//qDebug() << "Screen master =" << m_masterTime;

    /*
        Qt ekran yakalama

        windowId = 0
        tüm ekran
    */

    QPixmap pixmap =
            m_screen->grabWindow(0);




    if(pixmap.isNull())
    {
        qWarning() << "Screen grab failed.";
        return false;
    }


    QImage image =
            pixmap.toImage();


    /*qDebug()
        << image.size()
        << image.format()
        << image.isNull();*/

    if(image.isNull())
    {
        qWarning()
                << "Screen grab failed.";

        return false;
    }



    /*
       Şimdilik RGBA bırakıyoruz.
       VideoFrameConverter
       bunu YUV420P yapacak.
    */


   /* if (m_settings.width > 0 &&
        m_settings.height > 0 &&
        (image.width() != m_settings.width ||
         image.height() != m_settings.height))
    {
        int w = m_settings.width  & ~1;
        int h = m_settings.height & ~1;

        /*image = image.scaled(
            w,
            h,
            Qt::IgnoreAspectRatio,
            Qt::FastTransformation);*/
    //}*/

    image = image.convertToFormat(QImage::Format_RGBA8888);



    auto buffer =
        QSharedPointer<MediaBuffer>(
            new MediaBuffer);



    int size =
        image.sizeInBytes();



    buffer->storage.resize(size);



    memcpy(
        buffer->storage.data(),
        image.constBits(),
        size);



    buffer->plane[0] =
        reinterpret_cast<uint8_t*>(
            buffer->storage.data());



    buffer->planeCount = 1;


    buffer->stride[0] =
        image.bytesPerLine();



    frame.width =
        image.width();


    frame.height =
        image.height();


    //frame.format = PixelFormat::YUV420P;
    frame.format =PixelFormat::RGBA8888;



    frame.buffer = buffer;



    if(m_masterTime)
        frame.pts = m_masterTime->nowUs();
        //frame.pts =m_masterTime->timestamp();
//frame.pts = m_videoPts++;

    return true;
}



void ScreenCapture::release(MediaFrame &frame)
{
    Q_UNUSED(frame)

    /*
       Qt tarafında ekstra buffer yok.
       QSharedPointer otomatik temizler.
    */
}
