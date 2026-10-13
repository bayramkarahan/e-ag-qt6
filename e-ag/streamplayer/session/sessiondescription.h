#ifndef SESSIONDESCRIPTION_H
#define SESSIONDESCRIPTION_H

#include <QObject>
#include <QByteArray>
#include <QFile>

class SessionDescription : public QObject
{
    Q_OBJECT

public:
    explicit SessionDescription(QObject *parent=nullptr);

    void save(const QString &fileName);
    QByteArray toByteArray() const;
public slots:

    void setSps(const QByteArray &sps);
    void setPps(const QByteArray &pps);
    QString createSdp() const;

    void setVideoEnabled(bool enabled);
    void setAudioEnabled(bool enabled);
private:

    QByteArray m_sps;
    QByteArray m_pps;
    bool m_videoEnabled = false;
    bool m_audioEnabled = false;
};

#endif
