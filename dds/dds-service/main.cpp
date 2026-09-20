#include <QCoreApplication>

#include "ddsservice.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DDSService service;

    return a.exec();
}
