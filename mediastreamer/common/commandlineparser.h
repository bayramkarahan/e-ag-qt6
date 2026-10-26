#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QApplication>
#include "StreamSettings.h"
struct VideoQualityProfile
{
    int bitrate;
    int width;
    int height;
    int fps;
};
class CommandLineParser
{
public:
    static bool parse(QApplication &app,
                      StreamSettings &settings);
    static VideoQualityProfile applyVideoQuality(int quality);
};

#endif
