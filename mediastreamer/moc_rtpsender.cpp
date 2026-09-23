/****************************************************************************
** Meta object code from reading C++ file 'rtpsender.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "sender/rtpsender.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rtpsender.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_RtpSender_t {
    uint offsetsAndSizes[22];
    char stringdata0[10];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[10];
    char stringdata4[7];
    char stringdata5[15];
    char stringdata6[13];
    char stringdata7[5];
    char stringdata8[5];
    char stringdata9[18];
    char stringdata10[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_RtpSender_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_RtpSender_t qt_meta_stringdata_RtpSender = {
    {
        QT_MOC_LITERAL(0, 9),  // "RtpSender"
        QT_MOC_LITERAL(10, 10),  // "sendPacket"
        QT_MOC_LITERAL(21, 0),  // ""
        QT_MOC_LITERAL(22, 9),  // "RTPPacket"
        QT_MOC_LITERAL(32, 6),  // "packet"
        QT_MOC_LITERAL(39, 14),  // "addDestination"
        QT_MOC_LITERAL(54, 12),  // "QHostAddress"
        QT_MOC_LITERAL(67, 4),  // "addr"
        QT_MOC_LITERAL(72, 4),  // "port"
        QT_MOC_LITERAL(77, 17),  // "removeDestination"
        QT_MOC_LITERAL(95, 17)   // "clearDestinations"
    },
    "RtpSender",
    "sendPacket",
    "",
    "RTPPacket",
    "packet",
    "addDestination",
    "QHostAddress",
    "addr",
    "port",
    "removeDestination",
    "clearDestinations"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_RtpSender[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x0a,    1 /* Public */,
       5,    2,   41,    2, 0x0a,    3 /* Public */,
       9,    2,   46,    2, 0x0a,    6 /* Public */,
      10,    0,   51,    2, 0x0a,    9 /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6, QMetaType::UShort,    7,    8,
    QMetaType::Void, 0x80000000 | 6, QMetaType::UShort,    7,    8,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject RtpSender::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_RtpSender.offsetsAndSizes,
    qt_meta_data_RtpSender,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_RtpSender_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RtpSender, std::true_type>,
        // method 'sendPacket'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const RTPPacket &, std::false_type>,
        // method 'addDestination'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QHostAddress &, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint16, std::false_type>,
        // method 'removeDestination'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QHostAddress &, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint16, std::false_type>,
        // method 'clearDestinations'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void RtpSender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RtpSender *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sendPacket((*reinterpret_cast< std::add_pointer_t<RTPPacket>>(_a[1]))); break;
        case 1: _t->addDestination((*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<quint16>>(_a[2]))); break;
        case 2: _t->removeDestination((*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<quint16>>(_a[2]))); break;
        case 3: _t->clearDestinations(); break;
        default: ;
        }
    }
}

const QMetaObject *RtpSender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RtpSender::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RtpSender.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RtpSender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
