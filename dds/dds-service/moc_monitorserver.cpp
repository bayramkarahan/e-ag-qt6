/****************************************************************************
** Meta object code from reading C++ file 'monitorserver.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "monitorserver.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'monitorserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_MonitorServer_t {
    uint offsetsAndSizes[42];
    char stringdata0[14];
    char stringdata1[24];
    char stringdata2[1];
    char stringdata3[19];
    char stringdata4[2];
    char stringdata5[13];
    char stringdata6[7];
    char stringdata7[23];
    char stringdata8[8];
    char stringdata9[2];
    char stringdata10[24];
    char stringdata11[28];
    char stringdata12[26];
    char stringdata13[20];
    char stringdata14[17];
    char stringdata15[7];
    char stringdata16[16];
    char stringdata17[7];
    char stringdata18[20];
    char stringdata19[19];
    char stringdata20[28];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MonitorServer_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MonitorServer_t qt_meta_stringdata_MonitorServer = {
    {
        QT_MOC_LITERAL(0, 13),  // "MonitorServer"
        QT_MOC_LITERAL(14, 23),  // "processPendingDatagrams"
        QT_MOC_LITERAL(38, 0),  // ""
        QT_MOC_LITERAL(39, 18),  // "sendGetClientReply"
        QT_MOC_LITERAL(58, 1),  // "d"
        QT_MOC_LITERAL(60, 12),  // "QHostAddress"
        QT_MOC_LITERAL(73, 6),  // "sender"
        QT_MOC_LITERAL(80, 22),  // "sendClientAddedMonitor"
        QT_MOC_LITERAL(103, 7),  // "Client*"
        QT_MOC_LITERAL(111, 1),  // "c"
        QT_MOC_LITERAL(113, 23),  // "receivedMonitorSettings"
        QT_MOC_LITERAL(137, 27),  // "receivedMonitorReceiverData"
        QT_MOC_LITERAL(165, 25),  // "receivedMonitorSenderData"
        QT_MOC_LITERAL(191, 19),  // "sendProgressChanged"
        QT_MOC_LITERAL(211, 16),  // "receivedDebStart"
        QT_MOC_LITERAL(228, 6),  // "client"
        QT_MOC_LITERAL(235, 15),  // "receivedDebDone"
        QT_MOC_LITERAL(251, 6),  // "status"
        QT_MOC_LITERAL(258, 19),  // "receivedScriptStart"
        QT_MOC_LITERAL(278, 18),  // "receivedScriptDone"
        QT_MOC_LITERAL(297, 27)   // "receivedMonitorStopTransfer"
    },
    "MonitorServer",
    "processPendingDatagrams",
    "",
    "sendGetClientReply",
    "d",
    "QHostAddress",
    "sender",
    "sendClientAddedMonitor",
    "Client*",
    "c",
    "receivedMonitorSettings",
    "receivedMonitorReceiverData",
    "receivedMonitorSenderData",
    "sendProgressChanged",
    "receivedDebStart",
    "client",
    "receivedDebDone",
    "status",
    "receivedScriptStart",
    "receivedScriptDone",
    "receivedMonitorStopTransfer"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MonitorServer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   86,    2, 0x0a,    1 /* Public */,
       3,    2,   87,    2, 0x0a,    2 /* Public */,
       7,    1,   92,    2, 0x0a,    5 /* Public */,
      10,    2,   95,    2, 0x0a,    7 /* Public */,
      11,    2,  100,    2, 0x0a,   10 /* Public */,
      12,    2,  105,    2, 0x0a,   13 /* Public */,
      13,    1,  110,    2, 0x0a,   16 /* Public */,
      14,    1,  113,    2, 0x0a,   18 /* Public */,
      16,    2,  116,    2, 0x0a,   20 /* Public */,
      18,    1,  121,    2, 0x0a,   23 /* Public */,
      19,    2,  124,    2, 0x0a,   25 /* Public */,
      20,    2,  129,    2, 0x0a,   28 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 5,    4,    6,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 5,    4,    6,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   15,   17,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   15,   17,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 5,    4,    6,

       0        // eod
};

Q_CONSTINIT const QMetaObject MonitorServer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MonitorServer.offsetsAndSizes,
    qt_meta_data_MonitorServer,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_MonitorServer_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MonitorServer, std::true_type>,
        // method 'processPendingDatagrams'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendGetClientReply'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'sendClientAddedMonitor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Client *, std::false_type>,
        // method 'receivedMonitorSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'receivedMonitorReceiverData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'receivedMonitorSenderData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'sendProgressChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Client *, std::false_type>,
        // method 'receivedDebStart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'receivedDebDone'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'receivedScriptStart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'receivedScriptDone'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'receivedMonitorStopTransfer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>
    >,
    nullptr
} };

void MonitorServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MonitorServer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->processPendingDatagrams(); break;
        case 1: _t->sendGetClientReply((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 2: _t->sendClientAddedMonitor((*reinterpret_cast< std::add_pointer_t<Client*>>(_a[1]))); break;
        case 3: _t->receivedMonitorSettings((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 4: _t->receivedMonitorReceiverData((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 5: _t->receivedMonitorSenderData((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 6: _t->sendProgressChanged((*reinterpret_cast< std::add_pointer_t<Client*>>(_a[1]))); break;
        case 7: _t->receivedDebStart((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->receivedDebDone((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 9: _t->receivedScriptStart((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->receivedScriptDone((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 11: _t->receivedMonitorStopTransfer((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject *MonitorServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MonitorServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MonitorServer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MonitorServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
