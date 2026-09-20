#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include<QFileDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QDir>
#include <QFileInfo>
#include "monitorclient.h"
// --------------------------------------------------
// ARG STRUCT
// --------------------------------------------------
struct ParsedArgs
{
    QString mode = "custom";
    QString target = "tmp";
    QString filePath;
};
// -----------------------------
// TARGET PATH RESOLVER
// -----------------------------
// --------------------------------------------------
// ARG PARSER
// --------------------------------------------------
ParsedArgs parseArguments(QStringList args)
{
    ParsedArgs result;

    args.removeFirst(); // program adı

    for(int i = 0; i < args.size(); i++)
    {
        const QString &arg = args[i];

        if(arg == "--mode" && i + 1 < args.size())
        {
            result.mode = args[i + 1].toLower();
            i++;
        }
        else if(arg == "--target" && i + 1 < args.size())
        {
            result.target = args[i + 1].toLower();
            i++;
        }
        else
        {
            if(result.filePath.isEmpty())
                result.filePath = arg;
        }
    }

    return result;
}
//// qDebug() << "use:";
/// qDebug() << "dds-monitor --mode console --target home /dosya";


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    #ifdef Q_OS_LINUX
        QIcon icon(":/icons/dds.svg");
        a.setWindowIcon(icon);
    #endif
        MonitorClient *mc = new MonitorClient();

        ParsedArgs args = parseArguments(QCoreApplication::arguments());
        if(args.mode == "rightclick")
        {

            if( args.filePath.isEmpty())
                    {
                        qDebug() << "Hata: Dosya yolu yok!";
                        return -1;
                    }
            mc->filePathLE->setText(args.filePath.trimmed());

        }

    mc->setAttribute(Qt::WA_DeleteOnClose);
    mc->show();
    return a.exec();
}
