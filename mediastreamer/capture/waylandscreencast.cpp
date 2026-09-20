#include "waylandscreencast.h"

#include <QDBusArgument>
#include <QDBusConnectionInterface>
#include <QRandomGenerator>
#include <QVariant>
#include <QDebug>
#include<QCoreApplication>
namespace
{

const char *PORTAL_SERVICE =
    "org.freedesktop.portal.Desktop";

const char *PORTAL_PATH =
    "/org/freedesktop/portal/desktop";

const char *SCREENCAST_INTERFACE =
    "org.freedesktop.portal.ScreenCast";

const char *REQUEST_INTERFACE =
    "org.freedesktop.portal.Request";

}

WaylandScreenCast::WaylandScreenCast(QObject *parent)
    : QObject(parent),
      m_bus(QDBusConnection::sessionBus())
{
}

WaylandScreenCast::~WaylandScreenCast()
{
    if(m_pipeWireFd >= 0)
    {
        // PipeWire tarafı FD'nin sahibi olacak şekilde
        // kullanıldığında burada kapatılabilir.
        // Şimdilik PipeWire katmanına bırakıyoruz.
        m_pipeWireFd = -1;
    }
}

QString WaylandScreenCast::makeToken(
    const QString &prefix) const
{
    const quint32 random =
        QRandomGenerator::global()->generate();

    return QString("%1_%2_%3")
        .arg(prefix)
        .arg(QCoreApplication::applicationPid())
        .arg(random);
}

bool WaylandScreenCast::createSession()
{
    if(m_sessionCreated)
        return true;

    if(!m_bus.isConnected())
    {
        handleError(
            "Cannot connect to session D-Bus.");

        return false;
    }

    m_sessionToken =
        makeToken("mediastreamer_session");

    m_handleToken =
        makeToken("mediastreamer_handle");

    QDBusMessage message =
        QDBusMessage::createMethodCall(
            PORTAL_SERVICE,
            PORTAL_PATH,
            SCREENCAST_INTERFACE,
            "CreateSession");

    QVariantMap options;

    options.insert(
        "session_handle_token",
        m_sessionToken);

    options.insert(
        "handle_token",
        m_handleToken);

    message << options;

    QDBusMessage reply =
        m_bus.call(
            message,
            QDBus::BlockWithGui,
            5000);

    if(reply.type() == QDBusMessage::ErrorMessage)
    {
        handleError(
            QString("CreateSession failed: %1")
                .arg(reply.errorMessage()));

        return false;
    }

    if(reply.arguments().isEmpty())
    {
        handleError(
            "CreateSession returned no request path.");

        return false;
    }

    const QDBusObjectPath requestPath =
        reply.arguments()
            .at(0)
            .value<QDBusObjectPath>();

    const bool connected =
        m_bus.connect(
            PORTAL_SERVICE,
            requestPath.path(),
            REQUEST_INTERFACE,
            "Response",
            this,
            SLOT(onCreateSessionResponse(uint,QVariantMap)));

    if(!connected)
    {
        handleError(
            QString("Cannot connect CreateSession Response: %1")
                .arg(requestPath.path()));

        return false;
    }

    qDebug()
        << "CreateSession request:"
        << requestPath.path();

    return true;
}


void WaylandScreenCast::onCreateSessionResponse(
    uint response,
    const QVariantMap &results)
{
    qDebug()
        << "========== CreateSession Response ==========";

    qDebug()
        << "response =" << response;

    qDebug()
        << "result keys =" << results.keys();

    for(auto it = results.constBegin();
        it != results.constEnd();
        ++it)
    {
        qDebug()
            << "KEY:"
            << it.key()
            << "TYPE:"
            << it.value().typeName()
            << "VALUE:"
            << it.value();
    }

    qDebug()
        << "=============================================";

    if(response != 0)
    {
        handleError(
            QString("CreateSession failed. Response=%1")
                .arg(response));

        return;
    }

    QVariant value =
        results.value("session_handle");

    if(!value.isValid())
    {
        handleError(
            "CreateSession response does not contain session_handle.");

        return;
    }

    qDebug()
        << "session_handle QVariant:"
        << value
        << "type:"
        << value.typeName();

    QDBusObjectPath path;

    if(value.canConvert<QDBusObjectPath>())
    {
        path =
            value.value<QDBusObjectPath>();
    }
    else
    {
        qDebug()
            << "session_handle cannot convert to QDBusObjectPath.";

        QString str =
            value.toString();

        if(!str.isEmpty())
            path = QDBusObjectPath(str);
    }

    qDebug()
        << "session path:"
        << path.path();

    if(path.path().isEmpty())
    {
        handleError(
            "CreateSession returned empty session path.");

        return;
    }

    m_sessionPath = path;

    m_sessionCreated = true;

    qDebug()
        << "Wayland ScreenCast session created:"
        << m_sessionPath.path();

    emit sessionCreated();
}

bool WaylandScreenCast::selectSources(
    bool monitor,
    bool window,
    bool cursor)
{
    if(!m_sessionCreated)
    {
        handleError(
            "SelectSources called before CreateSession.");

        return false;
    }

    quint32 types = 0;

    if(monitor)
        types |= 1;

    if(window)
        types |= 2;

    if(types == 0)
    {
        handleError(
            "No source type selected.");

        return false;
    }

    QDBusMessage message =
        QDBusMessage::createMethodCall(
            PORTAL_SERVICE,
            PORTAL_PATH,
            SCREENCAST_INTERFACE,
            "SelectSources");

    QVariantMap options;

    options.insert(
        "types",
        types);

    // 1 = Hidden
    // 2 = Embedded
    // 4 = Metadata
    options.insert(
        "cursor_mode",
        cursor ? quint32(2)
               : quint32(1));

    options.insert(
        "multiple",
        false);

    message
        << QVariant::fromValue(m_sessionPath)
        << options;

    QDBusMessage reply =
        m_bus.call(
            message,
            QDBus::BlockWithGui,
            5000);

    if(reply.type() == QDBusMessage::ErrorMessage)
    {
        handleError(
            QString("SelectSources failed: %1")
                .arg(reply.errorMessage()));

        return false;
    }

    if(reply.arguments().isEmpty())
    {
        handleError(
            "SelectSources returned no request path.");

        return false;
    }
    const QDBusObjectPath requestPath =
        reply.arguments()
            .at(0)
            .value<QDBusObjectPath>();

    qDebug()
        << "SelectSources request:"
        << requestPath.path();

    const bool connected =
        m_bus.connect(
            PORTAL_SERVICE,
            requestPath.path(),
            REQUEST_INTERFACE,
            "Response",
            this,
            SLOT(onSelectSourcesResponse(uint,QVariantMap)));

    qDebug()
        << "SelectSources response signal connected:"
        << connected;

    if(!connected)
    {
        handleError(
            QString("Cannot connect SelectSources Response: %1")
                .arg(requestPath.path()));

        return false;
    }

    return true;
}

void WaylandScreenCast::onSelectSourcesResponse(
    uint response,
    const QVariantMap &results)
{
    qDebug()
        << "========== SelectSources Response ==========";

    qDebug()
        << "response =" << response;

    qDebug()
        << "result keys =" << results.keys();

    for(auto it = results.constBegin();
        it != results.constEnd();
        ++it)
    {
        qDebug()
            << "KEY:"
            << it.key()
            << "TYPE:"
            << it.value().typeName()
            << "VALUE:"
            << it.value();
    }

    qDebug()
        << "=============================================";

    if(response != 0)
    {
        handleError(
            QString("SelectSources failed. Response=%1")
                .arg(response));

        return;
    }

    m_sourcesSelected = true;

    qDebug()
        << "Wayland ScreenCast sources selected.";

    emit sourcesSelected();
}

bool WaylandScreenCast::start()
{
    if(!m_sessionCreated)
    {
        handleError(
            "Start called before CreateSession.");

        return false;
    }

    if(!m_sourcesSelected)
    {
        handleError(
            "Start called before SelectSources.");

        return false;
    }

    QDBusMessage message =
        QDBusMessage::createMethodCall(
            PORTAL_SERVICE,
            PORTAL_PATH,
            SCREENCAST_INTERFACE,
            "Start");

    QVariantMap options;

    message
        << QVariant::fromValue(m_sessionPath)
        << QString()
        << options;

    QDBusMessage reply =
        m_bus.call(
            message,
            QDBus::BlockWithGui,
            5000);

    if(reply.type() == QDBusMessage::ErrorMessage)
    {
        handleError(
            QString("Start failed: %1")
                .arg(reply.errorMessage()));

        return false;
    }

    if(reply.arguments().isEmpty())
    {
        handleError(
            "Start returned no request path.");

        return false;
    }

    const QDBusObjectPath requestPath =
        reply.arguments()
            .at(0)
            .value<QDBusObjectPath>();

    const bool connected =
        m_bus.connect(
            PORTAL_SERVICE,
            requestPath.path(),
            REQUEST_INTERFACE,
            "Response",
            this,
            SLOT(onStartResponse(uint,QVariantMap)));

    if(!connected)
    {
        handleError(
            QString("Cannot connect Start Response: %1")
                .arg(requestPath.path()));

        return false;
    }

    return true;
}

void WaylandScreenCast::onStartResponse(
    uint response,
    const QVariantMap &results)
{
    if(response != 0)
    {
        handleError(
            QString("Start failed. Response=%1")
                .arg(response));

        return;
    }

    qDebug()
        << "Start result keys:"
        << results.keys();

    for(auto it = results.constBegin();
        it != results.constEnd();
        ++it)
    {
        qDebug()
            << "KEY:"
            << it.key()
            << "VALUE:"
            << it.value()
            << "TYPE:"
            << it.value().typeName();
    }

    QVariant streamsVariant =
        results.value("streams");

    if(!streamsVariant.isValid())
    {
        handleError(
            "Start response does not contain streams.");

        return;
    }

    const QDBusArgument argument =
        streamsVariant.value<QDBusArgument>();

    if(argument.currentType() !=
       QDBusArgument::StructureType)
    {
        qDebug()
            << "streams argument type:"
            << argument.currentType();
    }

    QDBusArgument arrayArgument =
        streamsVariant.value<QDBusArgument>();

    arrayArgument.beginArray();

    bool found = false;

    while(!arrayArgument.atEnd())
    {
        arrayArgument.beginStructure();

        quint32 nodeId = 0;
        QVariantMap properties;

        arrayArgument
            >> nodeId
            >> properties;

        arrayArgument.endStructure();

        qDebug()
            << "ScreenCast stream:"
            << "nodeId =" << nodeId
            << "properties =" << properties;

        if(!found)
        {
            m_nodeId = nodeId;

            m_streamInfo.nodeId =
                nodeId;

            QVariant sourceType =
                properties.value(
                    "source_type");

            if(sourceType.isValid())
            {
                m_streamInfo.sourceType =
                    sourceType.toUInt();
            }

            QVariant size =
                properties.value("size");

            if(size.isValid())
            {
                const QSize s =
                    size.toSize();

                m_streamInfo.width =
                    s.width();

                m_streamInfo.height =
                    s.height();
            }

            found = true;
        }
    }

    arrayArgument.endArray();

    if(!found || m_nodeId == 0)
    {
        handleError(
            "No PipeWire stream returned by ScreenCast portal.");

        return;
    }

    m_started = true;

    qDebug()
        << "ScreenCast started."
        << "PipeWire node ID:"
        << m_nodeId;

    emit started();
}

bool WaylandScreenCast::openPipeWireRemote()
{
    if(!m_started)
    {
        handleError(
            "OpenPipeWireRemote called before Start.");

        return false;
    }

    QDBusMessage message =
        QDBusMessage::createMethodCall(
            PORTAL_SERVICE,
            PORTAL_PATH,
            SCREENCAST_INTERFACE,
            "OpenPipeWireRemote");

    QVariantMap options;

    message
        << QVariant::fromValue(m_sessionPath)
        << options;

    QDBusMessage reply =
        m_bus.call(
            message,
            QDBus::BlockWithGui,
            5000);

    if(reply.type() ==
       QDBusMessage::ErrorMessage)
    {
        handleError(
            QString(
                "OpenPipeWireRemote failed: %1")
                .arg(reply.errorMessage()));

        return false;
    }

    if(reply.arguments().isEmpty())
    {
        handleError(
            "OpenPipeWireRemote returned no FD.");

        return false;
    }

    QVariant fdVariant =
        reply.arguments().at(0);

    /*
     * D-Bus 'h' tipi Qt tarafında
     * QDBusUnixFileDescriptor olarak gelir.
     */

    QDBusUnixFileDescriptor fd =
        fdVariant.value<QDBusUnixFileDescriptor>();

    if(!fd.isValid())
    {
        handleError(
            "OpenPipeWireRemote returned invalid FD.");

        return false;
    }

    m_pipeWireFd =
        fd.fileDescriptor();

    m_pipeWireOpened = true;

    qDebug()
        << "PipeWire remote opened."
        << "FD =" << m_pipeWireFd;

    emit pipeWireRemoteOpened(
        m_pipeWireFd);

    emit ready();

    return true;
}

void WaylandScreenCast::handleError(
    const QString &message)
{
    qWarning()
        << "WaylandScreenCast:"
        << message;

    emit error(message);
}

bool WaylandScreenCast::isReady() const
{
    return m_sessionCreated &&
           m_sourcesSelected &&
           m_started &&
           m_pipeWireOpened &&
           m_nodeId != 0 &&
           m_pipeWireFd >= 0;
}

QString WaylandScreenCast::sessionHandle() const
{
    return m_sessionPath.path();
}

quint32 WaylandScreenCast::nodeId() const
{
    return m_nodeId;
}

int WaylandScreenCast::pipeWireFd() const
{
    return m_pipeWireFd;
}

WaylandScreenCast::StreamInfo
WaylandScreenCast::streamInfo() const
{
    return m_streamInfo;
}

