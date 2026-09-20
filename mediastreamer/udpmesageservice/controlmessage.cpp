#include "controlmessage.h"
#include<QIODevice>


namespace
{
    constexpr quint16 ProtocolVersion = 1;
}


QByteArray serializeControlMessage(
    const ControlMessage &message)
{
    QByteArray data;

    QDataStream stream(
        &data,
        QIODevice::WriteOnly);

   /* stream.setVersion(
        QDataStream::Qt_6_5);*/


    /*
     * Paket formatı:
     *
     * ProtocolVersion
     * MessageType
     * Sequence
     * SenderId
     * Payload
     */

    stream << ProtocolVersion;

    stream << static_cast<quint16>(
        message.type);

    stream << message.sequence;

    stream << message.senderId;

    stream << message.payload;


    return data;
}


bool deserializeControlMessage(
    const QByteArray &data,
    ControlMessage &message)
{
    QDataStream stream(data);

    /*stream.setVersion(
        QDataStream::Qt_6_5);*/


    quint16 protocolVersion = 0;
    quint16 messageType = 0;

    quint32 sequence = 0;
    quint32 senderId = 0;

    QByteArray payload;


    stream >> protocolVersion;


    if (stream.status() != QDataStream::Ok)
        return false;


    /*
     * Farklı protokol versiyonuysa
     * paketi kabul etme.
     */
    if (protocolVersion != ProtocolVersion)
        return false;


    stream >> messageType;
    stream >> sequence;
    stream >> senderId;
    stream >> payload;


    if (stream.status() != QDataStream::Ok)
        return false;


    message.type =
        static_cast<ControlMessageType>(
            messageType);

    message.sequence = sequence;

    message.senderId = senderId;

    message.payload = payload;


    return true;
}
