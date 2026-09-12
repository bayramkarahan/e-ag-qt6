#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QApplication>
#include "StreamSettings.h"

class CommandLineParser
{
public:
    static bool parse(QApplication &app,
                      StreamSettings &settings);
};

#endif
