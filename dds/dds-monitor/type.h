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
#include<QLabel>
#include<QProgressBar>
#include<QCheckBox>
#include<QMessageBox>
#include<QRandomGenerator>
#include<QRadioButton>
#include<QButtonGroup>
#include<QToolButton>

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
enum class SourceType
{
    File,
    Directory,
    Pattern
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

#define MULTICAST_IP "239.255.7.1"
#define UDP_PORT 40004
#define TCP_PORT 40006
#define MONITOR_CLIENT_PORT 60001
#define MONITOR_CLIENT_SERVER 60002

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

struct FileMeta
{
    quint32 type;
    quint64 transferId;
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
};


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
    qint64 packetId;
};


struct ClientRow
{
    QLabel *ip;
    QLabel *hostName;
    QLabel *role;
    QCheckBox *sendToSender;
    QCheckBox *sendToReceiver;
    QCheckBox *receiver;
    QCheckBox *getToOther;
    QProgressBar *progresBar;
    QLabel *speedLabel;
    QLabel *timeLabel;
    //QToolButton *stopButton;
    QCheckBox *select;

    qint64 diffTime;
    qint64 lastTime;
    qint64 startTime;
    qint64 totalFileSize = 0;
    quint64 transferId;
    QFrame *row;
};

inline  QMap<QString, ClientRow*> clients;
//inline  QMap<QString, Client*> clientsStore;
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

inline void log(const QString &msg)
{
    QString ts = QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
    qDebug() << ts << msg;
}
