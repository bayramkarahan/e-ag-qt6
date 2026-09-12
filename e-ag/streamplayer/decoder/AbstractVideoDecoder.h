#ifndef ABSTRACTVIDEODECODER_H
#define ABSTRACTVIDEODECODER_H

#include <QObject>

#include "../common/MediaFrame.h"
#include "../common/StreamSettings.h"

class AbstractVideoDecoder : public QObject
{
    Q_OBJECT
public:
    explicit AbstractVideoDecoder(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    virtual ~AbstractVideoDecoder() = default;
    enum class Type
       {
           CPU,
           VAAPI,
            QT
       };
    virtual Type type() const = 0;
    virtual bool start() = 0;
    virtual void setSettings(const StreamSettings &settings) = 0;
    virtual void stop() = 0;

public slots:
    virtual void decode(const QByteArray &nal,
                        qint64 pts,
                        bool marker) = 0;

signals:
    void frameReady(const MediaFrame &frame);
};
#endif
