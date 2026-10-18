#ifndef DESKTOPSESSION_H
#define DESKTOPSESSION_H

#include <QGuiApplication>
#include <QByteArray>
#include <QString>

enum class DesktopSession
{
    Wayland,
    X11,
    Unknown
};

inline DesktopSession detectDesktopSession()
{
    const QString session =
        qEnvironmentVariable("XDG_SESSION_TYPE").toLower();

    const QString platform =
        QGuiApplication::platformName().toLower();

    // Gerçek masaüstü oturumu öncelikli.
    // Wayland oturumunda Qt uygulaması xcb/XWayland kullanabilir.
    if (session == "wayland")
        return DesktopSession::Wayland;

    if (session == "x11")
        return DesktopSession::X11;

    // Ortam değişkeni yoksa Qt platformuna bak.
    if (platform == "wayland")
        return DesktopSession::Wayland;

    if (platform == "xcb")
        return DesktopSession::X11;

    return DesktopSession::Unknown;
}

#endif
