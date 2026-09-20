#include <QCoreApplication>
#include<QDebug>
#include<Database.h>
#include<QThread>
#include<QProcess>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QStringList args = QCoreApplication::arguments();

    if(args.size() < 8)
    {
        qDebug() << "Kullanım: set-role <role>";
        return 1;
    }

    QString command = args.at(1);

    if(command == "set-role")
    {
        QString role = args.at(2);
        QString preSendToSender = args.at(3);
        bool sendToSender = preSendToSender.toLower() == "true";
        QString preSendToReceiver= args.at(4);
        bool sendToReceiver = preSendToReceiver.toLower() == "true";
        QString preReceiver= args.at(5);
        bool receiver = preReceiver.toLower() == "true";
        QString preGetToOther= args.at(6);
        bool getToOther = preGetToOther.toLower() == "true";
        QString preRoot= args.at(7);
        bool root = preRoot.toLower() == "true";
        QString path = "/usr/share/dds/dds.json";

        DatabaseHelper db(path);

        QJsonObject obj;
        obj["role"] = role;
        obj["sendToSender"] = sendToSender;
        obj["sendToReceiver"] = sendToReceiver;
        obj["receiver"] = receiver;
        obj["getToOther"] = getToOther;
        obj["root"] = root;
        db.TekKayitYaz(obj);
        qDebug() << "Role yazıldı:" << role<<sendToSender<<sendToReceiver<<receiver<<getToOther;

        ///QThread::msleep(300);
        ///QProcess::execute("systemctl",{"restart", "dds-service.service"});
        return 0;
    }

    return 0;
}
