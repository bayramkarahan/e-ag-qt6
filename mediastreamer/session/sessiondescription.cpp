#include "sessiondescription.h"
static QByteArray removeStartCode(const QByteArray &nal)
{
    if (nal.startsWith("\x00\x00\x00\x01"))
        return nal.mid(4);

    if (nal.startsWith("\x00\x00\x01"))
        return nal.mid(3);

    return nal;
}

SessionDescription::SessionDescription(QObject *parent)
    : QObject(parent)
{
}


QString SessionDescription::createSdp() const
{
    QString s;

    s += "v=0\r\n";
    s += "o=- 0 0 IN IP4 127.0.0.1\r\n";
    s += "s=MediaStreamer\r\n";
    s += "t=0 0\r\n";

    s += "a=control:*\r\n";

    if (m_videoEnabled)
    {
        if (m_sps.isEmpty() || m_pps.isEmpty())
            return {};

        QByteArray sps =
            removeStartCode(m_sps).toBase64();

        QByteArray pps =
            removeStartCode(m_pps).toBase64();

        s += "m=video 5004 RTP/AVP 96\r\n";
        s += "a=control:trackID=0\r\n";
        s += "a=rtpmap:96 H264/90000\r\n";

        s += QString(
            "a=fmtp:96 "
            "packetization-mode=1;"
            "sprop-parameter-sets=%1,%2\r\n")
            .arg(QString::fromLatin1(sps))
            .arg(QString::fromLatin1(pps));
    }

    if (m_audioEnabled)
    {
        s += "m=audio 5006 RTP/AVP 97\r\n";
        s += "a=control:trackID=1\r\n";
        s += "a=rtpmap:97 opus/48000/2\r\n";
    }

    return s;
}void SessionDescription::setVideoEnabled(bool enabled)
{
    m_videoEnabled = enabled;
}

void SessionDescription::setAudioEnabled(bool enabled)
{
    m_audioEnabled = enabled;
}
void SessionDescription::setSps(const QByteArray &sps)
{
    m_sps = sps;
}

void SessionDescription::setPps(const QByteArray &pps)
{
    m_pps = pps;

    save("stream.sdp");
}

QByteArray SessionDescription::toByteArray() const
{
    return createSdp().toUtf8();
}

void SessionDescription::save(const QString &fileName)
{
    QString text = createSdp();

    if (text.isEmpty())
        return;

    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        file.write(text.toUtf8());
}
