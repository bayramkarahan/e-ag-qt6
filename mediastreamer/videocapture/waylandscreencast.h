#ifndef WAYLANDSCREENCAST_H
#define WAYLANDSCREENCAST_H

#include <QObject>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusObjectPath>
#include <QVariantMap>
#include <QDBusUnixFileDescriptor>
#include <QHash>
#include <QSize>
#include <QDBusPendingCallWatcher>
extern "C" {
#include <pipewire/pipewire.h>
#include <spa/param/video/format-utils.h>
}
class WaylandScreenCast : public QObject
{
    Q_OBJECT

public:

    struct StreamInfo
    {
        quint32 nodeId = 0;
        quint32 sourceType = 0;
        quint32 width = 0;
        quint32 height = 0;
        quint32 positionX = 0;
        quint32 positionY = 0;
        quint32 cursorMode = 0;
    };

    explicit WaylandScreenCast(QObject *parent = nullptr);
    ~WaylandScreenCast();
    static void onStateChanged(
          void *data,
          pw_stream_state oldState,
          pw_stream_state state,
          const char *error);

      static void onParamChanged(
          void *data,
          uint32_t id,
          const spa_pod *param);

      static void onProcess(void *data);

    bool createSession();

    bool selectSources(
        bool monitor = true,
        bool window = false,
        bool cursor = false);

    bool start();

    bool openPipeWireRemote();

    bool isReady() const;

    QString sessionHandle() const;

    quint32 nodeId() const;

    int pipeWireFd() const;

    StreamInfo streamInfo() const;

signals:

    void sessionCreated();

    void sourcesSelected();

    void started();

    void pipeWireRemoteOpened(int fd);

    void ready();

    void error(const QString &message);

private slots:

    void onCreateSessionResponse(
        uint response,
        const QVariantMap &results);

    void onSelectSourcesResponse(
        uint response,
        const QVariantMap &results);

    void onStartResponse(
        uint response,
        const QVariantMap &results);
    bool connectRequest(
        const QString &operation,
        const QDBusObjectPath &requestPath,
        const char *slot);

    void disconnectRequest(
        const QString &operation,
        const char *slot);
private:
    void handleError(const QString &message);
    QString makeToken(const QString &prefix) const;
    bool connectRequest(const QDBusObjectPath &requestPath,const char *slot);
    void disconnectRequest(const QDBusObjectPath &requestPath,const char *slot);

private:

    QDBusConnection m_bus;

    QString m_sessionToken;
    QString m_handleToken;

    QDBusObjectPath m_sessionPath;

    quint32 m_nodeId = 0;

    int m_pipeWireFd = -1;

    StreamInfo m_streamInfo;

    bool m_sessionCreated = false;
    bool m_sourcesSelected = false;
    bool m_started = false;
    bool m_pipeWireOpened = false;
        QHash<QString, QString> m_requests;
};

#endif // WAYLANDSCREENCAST_H
