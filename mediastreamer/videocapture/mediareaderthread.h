#ifndef MEDIAREADERTHREAD_H
#define MEDIAREADERTHREAD_H

#include <QThread>
#include <QAtomicInteger>

class MediaFileCapture;

class MediaReaderThread : public QThread
{
    Q_OBJECT

public:
    explicit MediaReaderThread(QObject *parent = nullptr);

    void setCapture(MediaFileCapture *capture);

    void stop();

protected:
    void run() override;

private:
    MediaFileCapture *m_capture = nullptr;

    QAtomicInteger<bool> m_running{false};
};

#endif
