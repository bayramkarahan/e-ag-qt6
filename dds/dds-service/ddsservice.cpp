#include "ddsservice.h"
#include "discoverymanager.h"
#include "transfermanager.h"
#include "filesender.h"
#include "filereceiver.h"
#include "monitorserver.h"
#include "udppacketrouter.h"
#include "normsender.h"
#include "normreceiver.h"

DDSService::DDSService(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<FileMeta>("FileMeta");
    log("DdsService STARTED");
    localIp=getLocalIp();
    roleInfo = getRole();

    udpPacketRouter = new UdpPacketRouter(this);
    discoveryManager = new DiscoveryManager(this,this);
    transferManager= new TransferManager(this,this);
    fileSender     = new FileSender(this);
    fileReceiver   = new FileReceiver(this);
    monitorServer  = new MonitorServer(this);
    normFileSender =new NormSender(this);
    normFileReceiver =new NormReceiver(this);
    normFileReceiver->start("/tmp",NORM_MULTICAST_IP,NORM_PORT);

    connect(normFileReceiver,&NormReceiver::fileReceived,
            transferManager,&TransferManager::normFileReceived);


    connect(udpPacketRouter,&UdpPacketRouter::getFileHomeRequestReceived,
                            discoveryManager,&DiscoveryManager::sendFileHome);

    connect(udpPacketRouter,&UdpPacketRouter::transferPacketReceived,
                            transferManager,&TransferManager::processDatagram);

    connect(udpPacketRouter,&UdpPacketRouter::discoveryReceived,
                            discoveryManager,&DiscoveryManager::receivedDiscovery);

    connect(udpPacketRouter,&UdpPacketRouter::discoveryReplyReceived,
                            discoveryManager,&DiscoveryManager::receivedDiscoveryReply);

    fileReceiver->listen(QHostAddress::Any,TCP_PORT);

    connect(fileReceiver,&FileReceiver::receiveFinished,
            transferManager,&TransferManager::onReceiveFinished);

    connect(fileSender,&FileSender::transferFinished,
            transferManager,&TransferManager::onSendFinished);

    /*connect(fileSender,&FileSender::transferProgress,
            transferManager,&TransferManager::receivedProgressChanged);*/

    connect(transferManager,&TransferManager::startFileSender,
            fileSender,&FileSender::sendFile);

    connect(discoveryManager,&DiscoveryManager::clientAddedMonitor,
            monitorServer,&MonitorServer::sendClientAddedMonitor);


    connect(transferManager,&TransferManager::debStart,
            monitorServer,&MonitorServer::receivedDebStart);
    connect(transferManager,&TransferManager::debDone,
            monitorServer,&MonitorServer::receivedDebDone);

    connect(transferManager,&TransferManager::scriptStart,
            monitorServer,&MonitorServer::receivedScriptStart);
    connect(transferManager,&TransferManager::scriptDone,
            monitorServer,&MonitorServer::receivedScriptDone);


    discoveryManager->sendDiscovery();
   /// QTimer::singleShot(2000,this,[this](){qDebug()<<clients.count();});


}

void DDSService::senderFile(QString filePath,QString destPath,
        SourceType sourceType,TargetLocation targetLocation,QStringList clientList)
{
    Q_UNUSED(sourceType)
    //QStringList sourcePaths;
    //sourcePaths << filePath;
    SourceType mySourceType;
    mySourceType=fileTypeDetect(filePath);
     QFileInfo info(filePath);
     QString sourceRoot=info.absolutePath();
     QString myfile=info.fileName();

    QStringList sourcePathList=sourcePaths(sourceRoot,myfile,mySourceType);
    //qDebug()<<filePath<<(quint32)mySourceType<<basefile<<sourceRoot;
    //qDebug()<<sourcePathList;
    //return;
    sendSourcePaths(sourcePathList,mySourceType,targetLocation,clientList,destPath);
}

void DDSService::receiverFile(QString fileName, SourceType sourceType, QStringList clientList, SourceLocation sourceLocation,TargetLocation targetLocation)
{

    discoveryManager->sendGetFile(roleInfo.role,fileName,sourceType,clientList,sourceLocation,targetLocation);
}


void DDSService::getFileDoProcess(
        QString myrole,
        QString myfile,
        SourceType mySourceType,
        QStringList clientList,
        SourceLocation sourceLocation,
        TargetLocation targetLocation)
{
    if(roleInfo.role != "receiver" ||
       myrole != "sender")
    {
        return;
    }

    QString sourceRoot =resolveSourceRoot(sourceLocation);
    //fileTypeDetect
    mySourceType=fileTypeDetect(sourceRoot+"/"+myfile);
    qDebug()<<"***********************************";
    qDebug()<<"kontrol edilen sourcePath"<<sourceRoot+"/"+myfile;

    qDebug()<<"tespit edilen sourcetype"<<(quint32)mySourceType;

    QStringList sourcePathList=sourcePaths(sourceRoot,myfile,mySourceType);

    sendSourcePaths(
        sourcePathList,
                mySourceType,
        targetLocation,
        clientList,
        QString());
}

QStringList DDSService::sourcePaths(QString sourceRoot, QString myfile,SourceType mySourceType)
{
    QStringList sourcePaths;

    if(mySourceType == SourceType::File)
    {
        sourcePaths
                << sourceRoot + "/" + myfile;
    }
    else if(mySourceType == SourceType::Directory)
    {
        sourcePaths
                << sourceRoot + "/" + myfile;
    }
    else if(mySourceType == SourceType::Pattern)
    {
        QDir dir(sourceRoot);

        QStringList files =
                dir.entryList(
                    QStringList() << myfile,
                    QDir::Files,
                    QDir::Name);

        if(files.isEmpty())
        {
            qDebug()
                    << "Pattern ile eşleşen dosya bulunamadı";
            return QStringList("");
        }

        for(const QString &file : files)
        {
            sourcePaths
                    << dir.absoluteFilePath(file);
        }
    }
    return sourcePaths;
}

QString DDSService::resolveSourceRoot(SourceLocation location)
{
    UserPrivilegeHelper helper;
    SessionInfo info = helper.getActiveSessionInfo();

    QString home = info.home;
    QString desktop = getDesktopPathFromHome(home);

    switch(location)
    {
    case SourceLocation::Desktop:
        return desktop;

    case SourceLocation::Home:
        return home;

    case SourceLocation::Tmp:
        return "/tmp";

    case SourceLocation::Custom:
        return "";
    }

    return "";
}

bool DDSService::copyRecursively(const QString &srcPath,const QString &dstPath)
{
    QFileInfo srcFileInfo(srcPath);

    if(srcFileInfo.isDir())
    {
        QDir targetDir(dstPath);

        if(!targetDir.exists())
        {
            if(!targetDir.mkpath("."))
                return false;
        }

        QDir sourceDir(srcPath);

        QFileInfoList entries =
            sourceDir.entryInfoList(
                QDir::NoDotAndDotDot |
                QDir::AllEntries);

        for(const QFileInfo &entry : entries)
        {
            QString srcItem =entry.absoluteFilePath();

            QString dstItem =dstPath + "/" + entry.fileName();

            if(!copyRecursively(srcItem, dstItem))
                return false;
        }

        return true;
    }

    QFile::remove(dstPath);

    if(::link(srcPath.toUtf8().constData(),
              dstPath.toUtf8().constData()) == 0)
    {
        qDebug() << "HARDLINK:" << srcPath;
        return true;
    }

    qDebug() << "COPY:" << srcPath;


    // Hardlink başarısızsa normal kopya
    return QFile::copy(srcPath, dstPath);
}


QString DDSService::createArchive(
        const QStringList &sourcePaths)
{
    QString hostName =
            QHostInfo::localHostName();

    QString timestamp =
            QDateTime::currentDateTime()
            .toString("yyyy-MM-dd_hh-mm-ss");

    QString tempRoot =
            QString("/tmp/dds_%1")
            .arg(QRandomGenerator::global()->generate64());

    QString packageDir =
            tempRoot
            + "/DDS-Depo/"
            + hostName
            + "/"
            + timestamp;

    QDir().mkpath(packageDir);

    for(const QString &path : sourcePaths)
    {
        QFileInfo fi(path);

        QString target =
                packageDir + "/" + fi.fileName();

        copyRecursively(path, target);

    }

    QString archiveName =
            QString("/tmp/dds_%1.tar")
            .arg(QRandomGenerator::global()->generate64());

    QProcess tar;

    tar.setWorkingDirectory(tempRoot);
    QElapsedTimer timer;
    timer.start();
    tar.start("tar",
              {
                  "-cf",
                  archiveName,
                  "DDS-Depo"
              });

    tar.waitForFinished(-1);
    qDebug()<< "TAR CREATE"<< timer.elapsed()<< "ms";

    if(tar.exitCode() != 0)
    {
        qDebug() << tar.readAllStandardError();

        QDir(tempRoot).removeRecursively();

        return QString();
    }

    QDir(tempRoot).removeRecursively();

    return archiveName;
}

void DDSService::sendSourcePaths(const QStringList &sourcePaths,SourceType sourceType,TargetLocation targetLocation,
        const QStringList &clientList,const QString &destPath)
{
    QString lastName;
    if(sourceType!=SourceType::File)
    {
        lastName = createArchive(sourcePaths);

    }else if(sourcePaths.size()==1)
    {
        lastName=sourcePaths.first();
    }


    if(lastName.isEmpty())
    {
        qDebug() << "Arşiv oluşturulamadı";
        return;
    }

    quint64 tid =QRandomGenerator::global()->generate64();

    FileMeta fm;

    fm.transferId = tid;
    fm.sourcePath = lastName;

    // artık her şey tar olduğu için
    fm.sourceType = sourceType;

    fm.targetLocation = targetLocation;
    fm.allowedClients = clientList;
    fm.transferOverwrite = true;

    fm.targetTempPath = "/tmp";
    fm.targetDestinationPath = destPath;
    QString timestamp =QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    fm.targetRootPath="/DDS-Depo/"+ QHostInfo::localHostName()+ "/"+ timestamp;

    ///qDebug() << "önce sourceType:" << (quint32)fm.sourceType;
    fm=fileNameDetect(fm);
    /*qDebug() << "Gönderilecek sourceType:" << (quint32)fm.sourceType;
    qDebug() << "Gönderilecek arşiv:" << lastName;
    qDebug() << "sourceBaseName:" << fm.sourceBaseName;
    qDebug() << "sourceBasePath:" << fm.sourceBasePath;
    qDebug() << "targetRootPath:" << fm.targetRootPath;
    qDebug() << "targetLocation:" << (quint32)fm.targetLocation;
*/
    transferManager->sendMeta(fm);
}
