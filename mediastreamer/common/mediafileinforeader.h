#ifndef MEDIAFILEINFOREADER_H
#define MEDIAFILEINFOREADER_H

#include <QString>

struct MediaFileInfo
{
    bool valid = false;

    bool hasVideo = false;
    bool hasAudio = false;

    int width = 0;
    int height = 0;

    double fps = 0.0;

    QString videoCodec;
    QString audioCodec;

    int sampleRate = 0;
    int channels = 0;
};

class MediaFileInfoReader
{
public:
    static MediaFileInfo read(const QString &fileName);
};

#endif // MEDIAFILEINFOREADER_H
