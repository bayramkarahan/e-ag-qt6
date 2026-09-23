/****************************************************************************
** Meta object code from reading C++ file 'udppacketrouter.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "udppacketrouter.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'udppacketrouter.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_UdpPacketRouter_t {
    uint offsetsAndSizes[26];
    char stringdata0[16];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[2];
    char stringdata4[13];
    char stringdata5[7];
    char stringdata6[30];
    char stringdata7[27];
    char stringdata8[23];
    char stringdata9[23];
    char stringdata10[18];
    char stringdata11[13];
    char stringdata12[24];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_UdpPacketRouter_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_UdpPacketRouter_t qt_meta_stringdata_UdpPacketRouter = {
    {
        QT_MOC_LITERAL(0, 15),  // "UdpPacketRouter"
        QT_MOC_LITERAL(16, 16),  // "datagramReceived"
        QT_MOC_LITERAL(33, 0),  // ""
        QT_MOC_LITERAL(34, 1),  // "d"
        QT_MOC_LITERAL(36, 12),  // "QHostAddress"
        QT_MOC_LITERAL(49, 6),  // "sender"
        QT_MOC_LITERAL(56, 29),  // "getFileDesktopRequestReceived"
        QT_MOC_LITERAL(86, 26),  // "getFileHomeRequestReceived"
        QT_MOC_LITERAL(113, 22),  // "transferPacketReceived"
        QT_MOC_LITERAL(136, 22),  // "discoveryReplyReceived"
        QT_MOC_LITERAL(159, 17),  // "discoveryReceived"
        QT_MOC_LITERAL(177, 12),  // "metaReceived"
        QT_MOC_LITERAL(190, 23)   // "processPendingDatagrams"
    },
    "UdpPacketRouter",
    "datagramReceived",
    "",
    "d",
    "QHostAddress",
    "sender",
    "getFileDesktopRequestReceived",
    "getFileHomeRequestReceived",
    "transferPacketReceived",
    "discoveryReplyReceived",
    "discoveryReceived",
    "metaReceived",
    "processPendingDatagrams"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_UdpPacketRouter[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   62,    2, 0x06,    1 /* Public */,
       6,    2,   67,    2, 0x06,    4 /* Public */,
       7,    2,   72,    2, 0x06,    7 /* Public */,
       8,    2,   77,    2, 0x06,   10 /* Public */,
       9,    2,   82,    2, 0x06,   13 /* Public */,
      10,    2,   87,    2, 0x06,   16 /* Public */,
      11,    2,   92,    2, 0x06,   19 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      12,    0,   97,    2, 0x08,   22 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 4,    3,    5,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject UdpPacketRouter::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_UdpPacketRouter.offsetsAndSizes,
    qt_meta_data_UdpPacketRouter,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_UdpPacketRouter_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<UdpPacketRouter, std::true_type>,
        // method 'datagramReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'getFileDesktopRequestReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'getFileHomeRequestReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'transferPacketReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'discoveryReplyReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'discoveryReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'metaReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'processPendingDatagrams'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void UdpPacketRouter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UdpPacketRouter *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->datagramReceived((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 1: _t->getFileDesktopRequestReceived((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 2: _t->getFileHomeRequestReceived((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 3: _t->transferPacketReceived((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 4: _t->discoveryReplyReceived((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 5: _t->discoveryReceived((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 6: _t->metaReceived((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 7: _t->processPendingDatagrams(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UdpPacketRouter::*)(QByteArray , QHostAddress );
            if (_t _q_method = &UdpPacketRouter::datagramReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UdpPacketRouter::*)(QByteArray , QHostAddress );
            if (_t _q_method = &UdpPacketRouter::getFileDesktopRequestReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UdpPacketRouter::*)(QByteArray , QHostAddress );
            if (_t _q_method = &UdpPacketRouter::getFileHomeRequestReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (UdpPacketRouter::*)(QByteArray , QHostAddress );
            if (_t _q_method = &UdpPacketRouter::transferPacketReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (UdpPacketRouter::*)(QByteArray , QHostAddress );
            if (_t _q_method = &UdpPacketRouter::discoveryReplyReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (UdpPacketRouter::*)(QByteArray , QHostAddress );
            if (_t _q_method = &UdpPacketRouter::discoveryReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (UdpPacketRouter::*)(QByteArray , QHostAddress );
            if (_t _q_method = &UdpPacketRouter::metaReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject *UdpPacketRouter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UdpPacketRouter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UdpPacketRouter.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int UdpPacketRouter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void UdpPacketRouter::datagramReceived(QByteArray _t1, QHostAddress _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UdpPacketRouter::getFileDesktopRequestReceived(QByteArray _t1, QHostAddress _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UdpPacketRouter::getFileHomeRequestReceived(QByteArray _t1, QHostAddress _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void UdpPacketRouter::transferPacketReceived(QByteArray _t1, QHostAddress _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void UdpPacketRouter::discoveryReplyReceived(QByteArray _t1, QHostAddress _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void UdpPacketRouter::discoveryReceived(QByteArray _t1, QHostAddress _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void UdpPacketRouter::metaReceived(QByteArray _t1, QHostAddress _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
