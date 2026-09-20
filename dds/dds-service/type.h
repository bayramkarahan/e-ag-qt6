#pragma once
#include <QTcpSocket>
#include<QDateTime>
#include <QDebug>
#include <QUdpSocket>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QFile>
#include<QDataStream>
#include<Database.h>
#include <QFileInfo>
#include <QHostInfo>
#include<QProcess>
#include<userprivilegehelper.h>
#include<QDir>
#include <QObject>
#include<QUdpSocket>
#include <unistd.h>
#include <sys/stat.h>
#include <QDirIterator>
#include<QDir>
#include<QStandardPaths>
#include<QLocalServer>
#include<QLocalSocket>
#include<QRandomGenerator>
#include<QElapsedTimer>
#include <arpa/inet.h>
#include <cstring>
#define MULTICAST_IP "239.255.7.1"
#define NORM_MULTICAST_IP "239.255.8.1"
//#define SEND_PORT 40004
//#define GET_PORT 40005
#define UDP_PORT 40004
#define MONITOR_CLIENT_PORT 60001
#define MONITOR_CLIENT_SERVER 60002
#define TCP_PORT 40006
#define NORM_PORT 50006
#define META 0
#define DATA 1
#define END 2
#define NACK 3
#define DONE 4
#define PROGRESS 5

#define DEB_START         6
#define DEB_DONE          7
#define SCRIPT_START      8
#define SCRIPT_DONE       9
#define DISCOVERY 10
#define DISCOVERY_REPLY 11
#define FINALYSENTDONE 12
#define GET_FILE 15
#define GET_FILE_HOME 16
#define META_REPLY 17
#define DONE_REPLY 18
#define DO_DONE_PROCESS 19
#define STOP_TRANSFER 20

inline QString localDir="/usr/share/dds/";

enum class SourceType
{
    File,
    Directory,
    Pattern
};

enum class TargetLocation {
    FileCopyDesktop,
    FileCopyHome,
    FileCopyTmp,
    FileCopyCustom,
    DebInstall,
    ScriptExecute
};

enum class SourceLocation
{
    Desktop,
    Home,
    Tmp,
    Custom
};


enum MONITOR
{
    MONITOR_CLIENT_ADD = 101,
    CLIENT_REMOVE = 102,
    TRANSFER_START = 103,
    MONITOR_PROGRESS_CHANGED = 104,
    TRANSFER_DONE = 105,
    MONITOR_DISCOVERY= 106,
    MONITOR_DISCOVERY_REPLY= 107,
    MONITOR_SENDER_DATA= 108,
    MONITOR_RECEIVER_DATA= 109,
    MONITOR_SETTINGS= 110,
    MONITOR_ROLE_STATUS= 111,
    MONITOR_STOP_TRANSFER= 112
};


struct RoleInfo
{
    QString role;
    bool sendToSender;
    bool sendToReceiver;
    bool receiver;
    bool getToOther;
    bool root;
};

struct FileMeta
{
    quint32 type;
    quint64 transferId=0;
    QString sourcePath;
    SourceType sourceType;
    QString sourceBaseName;
    QString sourceBasePath;
    QString targetTempPath;
    QString targetDestinationPath;
    TargetLocation targetLocation;
    bool transferOverwrite;
    QStringList allowedClients;
    quint64 totalFileSize;
    bool overwrite;
    bool allowed;
    QString targetRootPath;
};
Q_DECLARE_METATYPE(FileMeta)
inline QHash<quint64, FileMeta> transferMap;
inline QHash<quint64, bool> normSenderFileMap;
struct TransferSession
{
    FileMeta fileMeta;
    QTcpSocket *socket = nullptr;
    ///quint64 transferId = 0;
    QString ip;
    QString filename;
    QFile *file = nullptr;
    qint64 offset = 0;
    qint64 totalSize = 0;

    bool headerSent = false;
    bool completed = false;
    int lastProgress = -1;
    int chunkSize = 512 * 1024;
};


struct ReceiverSession
{
    FileMeta fileMeta;
    QFile file;

    QString filename;
    quint64 transferId = 0;
    qint64 totalSize = 0;
    qint64 received = 0;
    bool completed = false;
    bool headerRead = false;
    QByteArray buffer;
    int lastProgress = -1;

    qint64 startTime = 0;

    qint64 lastBytes = 0;
    qint64 lastTime = 0;
    QByteArray headerBuffer;
};

inline void log(const QString &msg)
{
    QString ts = QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
    qDebug() << ts << msg;
}




struct Client
{
    QString ip;
    QString hostName;
    bool completed;
    QString role;
    bool rolesendToSender;
    bool rolesendToReceiver;
    bool rolereceiver;
    bool rolegetToOther;
    int totalSize;
    int percent;
    qint64 received;
    int speedData;
    int elapsedSec;
    int remainSec;
    quint64 transferId;
    QString fileName;
    QString serverIp;
    quint64 packetId;
    quint64 discoveryTime;
};


///inline QMap<QString, Client*> clients;
inline RoleInfo getRole()
{
    localDir="/usr/share/dds/";
    DatabaseHelper db(localDir+"dds.json");
    QJsonArray dizi=db.Oku();
    RoleInfo info;
    if(dizi.count()>0)
    {
        QJsonValue item=dizi.first();
        QJsonObject veri=item.toObject();

        info.role=veri["role"].toString();
        info.sendToSender=veri["sendToSender"].toBool();
        info.sendToReceiver=veri["sendToReceiver"].toBool();
        info.receiver=veri["receiver"].toBool();
        info.getToOther=veri["getToOther"].toBool();
        info.root=veri["root"].toBool();
        return info;
    }
    return info;
}


inline QString getLocalIp()
{
    for (const QNetworkInterface &iface : QNetworkInterface::allInterfaces())
    {
        if (!(iface.flags() & QNetworkInterface::IsUp) ||
            !(iface.flags() & QNetworkInterface::IsRunning))
            continue;

        for (const QNetworkAddressEntry &entry : iface.addressEntries())
        {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                entry.ip() != QHostAddress::LocalHost)
            {
                return entry.ip().toString();
            }
        }
    }
    return "";
}


inline SourceType fileTypeDetect(const QString &path)
{
    QString text = path.trimmed();

    // Wildcard içeriyorsa Pattern
    if (text.contains('*') ||
        text.contains('?') ||
        text.contains('[') ||
        text.contains(']'))
    {
        return SourceType::Pattern;
    }

    QFileInfo info(text);

    if (info.exists())
    {
        if (info.isDir())
            return SourceType::Directory;

        if (info.isFile())
            return SourceType::File;
    }

    // Yol mevcut değilse son karakterden tahmin et
    if (text.endsWith('/') || text.endsWith('\\'))
        return SourceType::Directory;

    return SourceType::File;
}



///inline void fileNameDetect(QString path)
inline FileMeta fileNameDetect(FileMeta fm)
{
    QFileInfo info(fm.sourcePath);
    if(info.exists())
    {
        fm.sourceBaseName = info.fileName();
        fm.sourceBasePath=info.path();
       /* if(info.isFile())
        {
            fm.sourceType=SourceType::File;
        }
        else if(info.isDir())
        {
            fm.sourceType=SourceType::Directory;
        }else
        {
            fm.sourceType=SourceType::Pattern;
        }*/
    }

    /* qDebug() << "sourcePath"<<fileMeta.sourcePath;
    qDebug() << "sourceBasePath"<<fileMeta.sourceBasePath;
    qDebug() << "sourceBaseName"<<fileMeta.sourceBaseName;
    qDebug() << "sourceType"<<fileMeta.sourceType;*/
    return fm;
}

inline QString normalizeIp(const QHostAddress &addr)
{
    quint32 ipv4 = addr.toIPv4Address();

    if (ipv4)
        return QHostAddress(ipv4).toString();

    return addr.toString();
}
inline QString  getDesktopPathFromHome(const QString &home)
{
    QFile file(home + "/.config/user-dirs.dirs");

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while(!file.atEnd())
        {
            QString line = file.readLine();

            if(line.startsWith("XDG_DESKTOP_DIR"))
            {
                int firstQuote = line.indexOf('"');
                int lastQuote = line.lastIndexOf('"');

                if(firstQuote != -1 && lastQuote != -1 && lastQuote > firstQuote)
                {
                    QString path = line.mid(firstQuote + 1, lastQuote - firstQuote - 1);
                    path.replace("$HOME", home);
                    return path;
                }
            }
        }
    }

    // fallback
    return home + "/Desktop";
}

inline QString resolveTargetPath(TargetLocation type, const QString& customPath)
{
    UserPrivilegeHelper helper;
    SessionInfo info = helper.getActiveSessionInfo();
    QString home = info.home;
    QString desktop = getDesktopPathFromHome(home);
    /* if (info.valid) {
         qDebug() << "Kullanıcı:" << info.username;
         qDebug() << "UID/GID:" << info.uid << "/" << info.gid;
         qDebug() << "Home:" << info.home;
         qDebug() << "Display:" << info.display;
         qDebug() << "Type:" << info.type;
         qDebug() << "Service:" << info.service;
     }*/

    //QString user = getActiveUser();
    //qDebug() << "Active user:" << user;
    //QString home="/home/"+user+"/";


    switch(type)
    {
        case TargetLocation::FileCopyDesktop:
            return desktop;

        case TargetLocation::FileCopyHome:
            return home;

    case TargetLocation::FileCopyTmp:
        return "/tmp";

        case TargetLocation::FileCopyCustom:
            return customPath;

        default:
            return "/tmp";
    }
}


inline QString detectNetworkType()
{
    QString activeIface;
    for (const QNetworkInterface &iface : QNetworkInterface::allInterfaces())
    {
        //log(QString("%1 | %2")
        //   .arg(iface.name())
        // .arg(iface.humanReadableName()));

        // sadece çalışanlar
        if (!(iface.flags() & QNetworkInterface::IsUp) ||
            !(iface.flags() & QNetworkInterface::IsRunning))
            continue;

        // loopback skip
        if (iface.flags() & QNetworkInterface::IsLoopBack)
            continue;

        // IP var mı?
        for (const QNetworkAddressEntry &entry : iface.addressEntries())
        {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
            {
                activeIface = iface.name();
                //log("ACTIVE IFACE: " + activeIface);

                // 🔥 Linux naming standard
                if (activeIface.startsWith("wl"))
                    {//return "wifi";
                    return activeIface;
                }

                if (activeIface.startsWith("en") || activeIface.startsWith("eth"))
                   {// return "ethernet";
                    return activeIface;
                }

                return "unknown";
            }
        }
    }

    return "unknown";
}
