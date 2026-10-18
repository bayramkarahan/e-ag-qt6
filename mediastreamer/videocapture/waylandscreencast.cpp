#include "waylandscreencast.h"
#include <QSettings>
#include <QDBusPendingCall>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QDBusArgument>
#include <QRandomGenerator>
#include <QVariant>
#include <QDebug>
#include <QDBusArgument>
#include <QDBusMetaType>
#include <QMetaType>
#include <QVariantMap>
#include <QList>
#include <QDebug>
#include <unistd.h>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>

struct PortalStream
{
    uint nodeId = 0;
    QVariantMap properties;
};

Q_DECLARE_METATYPE(PortalStream)

QDBusArgument &operator<<(QDBusArgument &argument,
                          const PortalStream &stream)
{
    argument.beginStructure();
    argument << stream.nodeId << stream.properties;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument,
                                PortalStream &stream)
{
    argument.beginStructure();
    argument >> stream.nodeId >> stream.properties;
    argument.endStructure();
    return argument;
}

namespace
{
const QString kService =
    QStringLiteral("org.freedesktop.portal.Desktop");

const QString kPortalPath =
    QStringLiteral("/org/freedesktop/portal/desktop");

const QString kScreenCastInterface =
    QStringLiteral("org.freedesktop.portal.ScreenCast");

const QString kRequestInterface =
    QStringLiteral("org.freedesktop.portal.Request");

const QString kSessionInterface =
    QStringLiteral("org.freedesktop.portal.Session");

QString makeRandomToken(const QString &prefix)
{
    return prefix + QStringLiteral("_%1")
                        .arg(QRandomGenerator::global()->generate());
}

// D-Bus'tan gelen (ii) biçimindeki bir çifti okumaya çalışır.
bool readPair(const QVariant &value, quint32 &first, quint32 &second)
{
    if (!value.isValid())
        return false;

    if (value.canConvert<QDBusArgument>()) {
        QDBusArgument argument = value.value<QDBusArgument>();

        int a = 0;
        int b = 0;

        argument.beginStructure();
        argument >> a >> b;
        argument.endStructure();

        if (a < 0 || b < 0)
            return false;

        first = static_cast<quint32>(a);
        second = static_cast<quint32>(b);
        return true;
    }

    const QVariantList list = value.toList();

    if (list.size() >= 2) {
        bool okA = false;
        bool okB = false;

        const uint a = list.at(0).toUInt(&okA);
        const uint b = list.at(1).toUInt(&okB);

        if (okA && okB) {
            first = a;
            second = b;
            return true;
        }
    }

    return false;
}
}

WaylandScreenCast::WaylandScreenCast(QObject *parent)
    : QObject(parent),
      m_bus(QDBusConnection::sessionBus())
{
    if (!m_bus.isConnected()) {
        qWarning() << "Session D-Bus bağlantısı kurulamadı.";
    }
}

WaylandScreenCast::~WaylandScreenCast()
{
    // Portal oturumunu kapat.
    if (!m_sessionPath.path().isEmpty()) {
        QDBusMessage message = QDBusMessage::createMethodCall(
            kService,
            m_sessionPath.path(),
            kSessionInterface,
            QStringLiteral("Close"));

        m_bus.asyncCall(message);
    }

    // takeFileDescriptor() ile sahiplenilen FD bu sınıfa aittir.
    if (m_pipeWireFd >= 0) {
        ::close(m_pipeWireFd);
        m_pipeWireFd = -1;
    }
}

QString WaylandScreenCast::makeToken(const QString &prefix) const
{
    return makeRandomToken(prefix);
}

void WaylandScreenCast::handleError(const QString &message)
{
    qWarning() << "WaylandScreenCast:" << message;
    emit error(message);
}

bool WaylandScreenCast::connectRequest(
    const QString &operation,
    const QDBusObjectPath &requestPath,
    const char *slot)
{
    if (requestPath.path().isEmpty()) {
        handleError(operation +
                    QStringLiteral(": Portal boş Request yolu döndürdü."));
        return false;
    }

    // Önce aynı işlem için eski bağlantı varsa kaldır.
    disconnectRequest(operation, slot);

    const bool connected = m_bus.connect(
        kService,
        requestPath.path(),
        kRequestInterface,
        QStringLiteral("Response"),
        this,
        slot);

    if (!connected) {
        handleError(operation +
                    QStringLiteral(": Response sinyali bağlanamadı."));
        return false;
    }

    m_requests.insert(operation, requestPath.path());

    qDebug() << operation << "Request:" << requestPath.path();
    return true;
}


void WaylandScreenCast::disconnectRequest(
    const QString &operation,
    const char *slot)
{
    const QString path = m_requests.take(operation);

    if (path.isEmpty())
        return;

    m_bus.disconnect(
        kService,
        path,
        kRequestInterface,
        QStringLiteral("Response"),
        this,
        slot);
}

// ------------------------------------------------------------
// 1. CreateSession
// ------------------------------------------------------------


bool WaylandScreenCast::createSession()
{
    if (!m_bus.isConnected()) {
        handleError(QStringLiteral("Session D-Bus bağlantısı yok."));
        return false;
    }

    if (m_sessionCreated ||
        m_requests.contains(QStringLiteral("CreateSession"))) {
        handleError(QStringLiteral("CreateSession zaten başlatılmış."));
        return false;
    }

    m_handleToken = makeToken(QStringLiteral("request"));
    m_sessionToken = makeToken(QStringLiteral("session"));

    QVariantMap options;
    options.insert(QStringLiteral("handle_token"), m_handleToken);
    options.insert(QStringLiteral("session_handle_token"), m_sessionToken);

    QDBusMessage message = QDBusMessage::createMethodCall(
        kService,
        kPortalPath,
        kScreenCastInterface,
        QStringLiteral("CreateSession"));

    message << options;

    // Request yolu, D-Bus servis adından ve handle_token'dan oluşturulur.
    QString sender = m_bus.baseService();
    sender.remove(QLatin1Char(':'));
    sender.replace(QLatin1Char('.'), QLatin1Char('_'));

    const QString requestPath =
        QStringLiteral("%1/request/%2/%3")
            .arg(kPortalPath, sender, m_handleToken);

    // Response sinyaline, D-Bus çağrısından önce bağlan.
    if (!connectRequest(
            QStringLiteral("CreateSession"),
            QDBusObjectPath(requestPath),
            SLOT(onCreateSessionResponse(uint,QVariantMap)))) {
        return false;
    }

    auto *watcher = new QDBusPendingCallWatcher(
        m_bus.asyncCall(message), this);

    connect(watcher, &QDBusPendingCallWatcher::finished,
            this, [this, requestPath]
            (QDBusPendingCallWatcher *w)
    {
        QDBusPendingReply<QDBusObjectPath> reply = *w;
        w->deleteLater();

        if (reply.isError()) {
            disconnectRequest(
                QStringLiteral("CreateSession"),
                SLOT(onCreateSessionResponse(uint,QVariantMap)));

            handleError(
                QStringLiteral("CreateSession D-Bus hatası: %1")
                    .arg(reply.error().message()));
            return;
        }

        const QString actualPath = reply.value().path();

        qDebug() << "CreateSession Request yolu:"
                 << actualPath;

        if (actualPath != requestPath) {
            qWarning() << "CreateSession Request yolu uyuşmuyor:"
                       << "beklenen =" << requestPath
                       << "dönen =" << actualPath;
        }
    });

    // İstek asenkron olarak devam eder.
    return true;
}


void WaylandScreenCast::onCreateSessionResponse(
    uint response,
    const QVariantMap &results)
{
    disconnectRequest(
        QStringLiteral("CreateSession"),
        SLOT(onCreateSessionResponse(uint,QVariantMap)));

    if (response != 0) {
        handleError(response == 1
            ? QStringLiteral("Kullanıcı ekran paylaşım oturumunu iptal etti.")
            : QStringLiteral("CreateSession başarısız oldu."));
        return;
    }

    const QVariant value =
        results.value(QStringLiteral("session_handle"));

    QString path;

    if (value.canConvert<QDBusObjectPath>())
        path = value.value<QDBusObjectPath>().path();

    // Bazı Qt/D-Bus sürümlerinde sonuç QString gelebilir.
    if (path.isEmpty())
        path = value.toString();

    if (!path.startsWith(QLatin1Char('/'))) {
        handleError(QStringLiteral("Geçerli session_handle alınamadı."));
        return;
    }

    m_sessionPath = QDBusObjectPath(path);
    m_sessionCreated = true;

    qDebug() << "ScreenCast oturumu oluşturuldu:" << path;

    emit sessionCreated();
}

// ------------------------------------------------------------
// 2. SelectSources
// ------------------------------------------------------------




bool WaylandScreenCast::selectSources(
    bool monitor,
    bool window,
    bool cursor)
{
    if (!m_sessionCreated) {
        handleError(QStringLiteral(
            "SelectSources öncesinde oturum oluşturulmalı."));
        return false;
    }

    if (m_sourcesSelected ||
        m_requests.contains(QStringLiteral("SelectSources"))) {
        handleError(QStringLiteral(
            "SelectSources zaten başlatılmış."));
        return false;
    }

    quint32 types = 0;

    if (monitor)
        types |= 1;

    if (window)
        types |= 2;

    if (types == 0) {
        handleError(QStringLiteral(
            "En az bir kaynak türü seçilmelidir."));
        return false;
    }

    // Request yolunu önceden hesaplamak için token'ı sakla.
    const QString handleToken =
        makeToken(QStringLiteral("select"));

    QVariantMap options;
    options.insert(QStringLiteral("handle_token"), handleToken);
    options.insert(QStringLiteral("types"), types);
    options.insert(QStringLiteral("multiple"), false);
    options.insert(QStringLiteral("cursor_mode"),
                   cursor ? quint32(2) : quint32(1));
    options.insert(QStringLiteral("persist_mode"), quint32(2));

    QSettings settings(
        QStringLiteral("MediaStreamer"),
        QStringLiteral("MediaStreamer"));

    const QString restoreToken =
        settings.value(
            QStringLiteral("screenCast/restoreToken")).toString();

    if (!restoreToken.isEmpty()) {
        options.insert(QStringLiteral("restore_token"), restoreToken);
        qInfo() << "ScreenCast restore token mevcut.";
    }

    // Portal Request yolu: sinyal bağlantısını çağrıdan önce kur.
    QString sender = m_bus.baseService();
    sender.remove(QLatin1Char(':'));
    sender.replace(QLatin1Char('.'), QLatin1Char('_'));

    const QString expectedPath =
        QStringLiteral("%1/request/%2/%3")
            .arg(kPortalPath, sender, handleToken);

    const char *responseSlot =
        SLOT(onSelectSourcesResponse(uint,QVariantMap));

    if (!connectRequest(
            QStringLiteral("SelectSources"),
            QDBusObjectPath(expectedPath),
            responseSlot)) {
        return false;
    }

    QDBusMessage message = QDBusMessage::createMethodCall(
        kService,
        kPortalPath,
        kScreenCastInterface,
        QStringLiteral("SelectSources"));

    message << QVariant::fromValue(m_sessionPath)
            << options;

    auto *watcher = new QDBusPendingCallWatcher(
        m_bus.asyncCall(message), this);

    connect(watcher, &QDBusPendingCallWatcher::finished,
            this, [this, expectedPath, responseSlot]
                  (QDBusPendingCallWatcher *w)
    {
        QDBusPendingReply<QDBusObjectPath> reply = *w;
        w->deleteLater();

        qInfo() << "SelectSources D-Bus yanıtı geldi.";

        if (reply.isError()) {
            disconnectRequest(
                QStringLiteral("SelectSources"),
                responseSlot);

            handleError(QStringLiteral(
                "SelectSources D-Bus hatası: %1")
                .arg(reply.error().message()));
            return;
        }

        const QString actualPath = reply.value().path();

        qDebug() << "SelectSources beklenen Request yolu:"
                 << expectedPath;
        qDebug() << "SelectSources dönen Request yolu:"
                 << actualPath;

        if (actualPath != expectedPath) {
            qWarning() << "SelectSources Request yolu uyuşmuyor!";
        }
    });

    return true;
}



void WaylandScreenCast::onSelectSourcesResponse(
    uint response,
    const QVariantMap &results)
{
    Q_UNUSED(results);

    disconnectRequest(
        QStringLiteral("SelectSources"),
        SLOT(onSelectSourcesResponse(uint,QVariantMap)));

    if (response != 0) {
        handleError(response == 1
            ? QStringLiteral("Ekran kaynağı seçimi iptal edildi.")
            : QStringLiteral("SelectSources başarısız oldu."));
        return;
    }

    m_sourcesSelected = true;

    qDebug() << "Ekran kaynağı seçildi.";

    emit sourcesSelected();
}

// ------------------------------------------------------------
// 3. Start
// ------------------------------------------------------------

bool WaylandScreenCast::start()
{
    if (!m_sourcesSelected) {
        handleError(QStringLiteral(
            "Start öncesinde kaynak seçimi tamamlanmalı."));
        return false;
    }

    if (m_started || m_requests.contains(QStringLiteral("Start"))) {
        handleError(QStringLiteral("Start zaten başlatılmış."));
        return false;
    }

    QVariantMap options;
    options.insert(QStringLiteral("handle_token"),
                   makeToken(QStringLiteral("start")));

    QDBusMessage message = QDBusMessage::createMethodCall(
        kService,
        kPortalPath,
        kScreenCastInterface,
        QStringLiteral("Start"));

    // Parent window boş bırakılıyor.
    message << QVariant::fromValue(m_sessionPath)
            << QStringLiteral("")
            << options;

    auto *watcher = new QDBusPendingCallWatcher(
        m_bus.asyncCall(message), this);

    connect(watcher, &QDBusPendingCallWatcher::finished,
            this, [this](QDBusPendingCallWatcher *w)
    {
        QDBusPendingReply<QDBusObjectPath> reply = *w;
        w->deleteLater();

        if (reply.isError()) {
            handleError(QStringLiteral("Start D-Bus hatası: %1")
                            .arg(reply.error().message()));
            return;
        }

        connectRequest(
            QStringLiteral("Start"),
            reply.value(),
            SLOT(onStartResponse(uint,QVariantMap)));
    });

    return true;
}



void WaylandScreenCast::onStartResponse(
    uint response,
    const QVariantMap &results)
{
    disconnectRequest(
        QStringLiteral("Start"),
        SLOT(onStartResponse(uint,QVariantMap)));

    if (response != 0) {
        handleError(response == 1
            ? QStringLiteral(
                  "Ekran paylaşımı başlatma işlemi iptal edildi.")
            : QStringLiteral("Start başarısız oldu."));
        return;
    }

    // Portalın döndürdüğü kalıcı oturum token'ını kaydet.
    QSettings settings(
        QStringLiteral("MediaStreamer"),
        QStringLiteral("MediaStreamer"));

    const QString tokenKey =
        QStringLiteral("screenCast/restoreToken");

    const QString restoreToken =
        results.value(QStringLiteral("restore_token")).toString();

    if (!restoreToken.isEmpty()) {
        settings.setValue(tokenKey, restoreToken);
        settings.sync();

        if (settings.status() == QSettings::NoError) {
            qInfo() << "ScreenCast restore token kaydedildi.";
        } else {
            qWarning() << "ScreenCast restore token kaydedilemedi.";
        }
    } else {
        qInfo() << "Start yanıtında restore_token bulunamadı.";
    }

    const QVariant streams =
        results.value(QStringLiteral("streams"));

    if (!streams.isValid() ||
        !streams.canConvert<QDBusArgument>()) {
        handleError(QStringLiteral(
            "Start yanıtındaki streams verisi geçersiz."));
        return;
    }

    const QDBusArgument argument =
        streams.value<QDBusArgument>();

    if (argument.currentSignature() !=
        QStringLiteral("a(ua{sv})")) {
        handleError(QStringLiteral(
            "Beklenmeyen streams veri yapısı."));
        return;
    }

    QFile file(QStringLiteral("/tmp/wayland-streams.txt"));

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        handleError(QStringLiteral(
            "Streams tanılama dosyası açılamadı."));
        return;
    }

    QTextStream out(&file);

    out << "signature: "
        << argument.currentSignature() << '\n';

    uint nodeId = 0;
    QVariantMap properties;
    int count = 0;

    argument.beginArray();

    while (!argument.atEnd()) {
        uint currentNodeId = 0;
        QVariantMap currentProperties;

        argument.beginStructure();
        argument >> currentNodeId;
        argument >> currentProperties;
        argument.endStructure();

        if (count == 0) {
            nodeId = currentNodeId;
            properties = currentProperties;
        }

        out << "node_id: " << currentNodeId << '\n';
        out << "properties: "
            << QString::fromUtf8(
                   QJsonDocument::fromVariant(
                       currentProperties).toJson(
                           QJsonDocument::Compact))
            << '\n';

        ++count;
    }

    argument.endArray();

    out << "stream_count: " << count << '\n';
    file.close();

    if (count == 0 || nodeId == 0) {
        handleError(QStringLiteral(
            "Portal geçerli bir PipeWire node ID döndürmedi."));
        return;
    }

    m_nodeId = nodeId;
    m_streamInfo.nodeId = nodeId;

    qDebug() << "PipeWire node ID:" << m_nodeId;
    qDebug() << "Stream properties:" << properties;
    qDebug() << "Streams tanılama dosyası:"
             << "/tmp/wayland-streams.txt";

    m_started = true;
    emit started();
}


// ------------------------------------------------------------
// 4. OpenPipeWireRemote
// ------------------------------------------------------------

bool WaylandScreenCast::openPipeWireRemote()
{
    if (!m_started || m_sessionPath.path().isEmpty()) {
        handleError(QStringLiteral(
            "OpenPipeWireRemote öncesinde Start tamamlanmalı."));
        return false;
    }

    if (m_pipeWireOpened) {
        handleError(QStringLiteral(
            "PipeWire bağlantısı zaten açılmış."));
        return false;
    }

    QVariantMap options;

    QDBusMessage message = QDBusMessage::createMethodCall(
        kService,
        kPortalPath,
        kScreenCastInterface,
        QStringLiteral("OpenPipeWireRemote"));

    message << QVariant::fromValue(m_sessionPath)
            << options;

    auto *watcher = new QDBusPendingCallWatcher(
        m_bus.asyncCall(message), this);

    connect(watcher, &QDBusPendingCallWatcher::finished,
            this, [this](QDBusPendingCallWatcher *w)
    {
        QDBusPendingReply<QDBusUnixFileDescriptor> reply = *w;
        w->deleteLater();

        if (reply.isError()) {
            handleError(QStringLiteral(
                "OpenPipeWireRemote D-Bus hatası: %1")
                .arg(reply.error().message()));
            return;
        }

        QDBusUnixFileDescriptor descriptor = reply.value();

        if (!descriptor.isValid()) {
            handleError(QStringLiteral(
                "Portal geçerli bir PipeWire FD döndürmedi."));
            return;
        }

        // FD sahipliğini bu sınıfa devret.
        const int fd = descriptor.takeFileDescriptor();

        if (fd < 0) {
            handleError(QStringLiteral(
                "PipeWire FD sahipliği alınamadı."));
            return;
        }

        if (m_pipeWireFd >= 0)
            ::close(m_pipeWireFd);

        m_pipeWireFd = fd;
        m_pipeWireOpened = true;

        qDebug() << "PipeWire remote açıldı. FD:" << m_pipeWireFd;

        emit pipeWireRemoteOpened(m_pipeWireFd);
        emit ready();
    });

    return true;
}

// ------------------------------------------------------------
// Getters
// ------------------------------------------------------------

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

WaylandScreenCast::StreamInfo WaylandScreenCast::streamInfo() const
{
    return m_streamInfo;
}


