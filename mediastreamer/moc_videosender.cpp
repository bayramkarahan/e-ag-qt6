/****************************************************************************
** Meta object code from reading C++ file 'videosender.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "sender/videosender.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videosender.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_VideoSender_t {
    uint offsetsAndSizes[32];
    char stringdata0[12];
    char stringdata1[9];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[8];
    char stringdata5[11];
    char stringdata6[6];
    char stringdata7[10];
    char stringdata8[13];
    char stringdata9[5];
    char stringdata10[5];
    char stringdata11[13];
    char stringdata12[13];
    char stringdata13[14];
    char stringdata14[12];
    char stringdata15[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_VideoSender_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_VideoSender_t qt_meta_stringdata_VideoSender = {
    {
        QT_MOC_LITERAL(0, 11),  // "VideoSender"
        QT_MOC_LITERAL(12, 8),  // "spsReady"
        QT_MOC_LITERAL(21, 0),  // ""
        QT_MOC_LITERAL(22, 8),  // "ppsReady"
        QT_MOC_LITERAL(31, 7),  // "onFrame"
        QT_MOC_LITERAL(39, 10),  // "MediaFrame"
        QT_MOC_LITERAL(50, 5),  // "frame"
        QT_MOC_LITERAL(56, 9),  // "addClient"
        QT_MOC_LITERAL(66, 12),  // "QHostAddress"
        QT_MOC_LITERAL(79, 4),  // "addr"
        QT_MOC_LITERAL(84, 4),  // "port"
        QT_MOC_LITERAL(89, 12),  // "removeClient"
        QT_MOC_LITERAL(102, 12),  // "clearClients"
        QT_MOC_LITERAL(115, 13),  // "setMasterTime"
        QT_MOC_LITERAL(129, 11),  // "MasterTime*"
        QT_MOC_LITERAL(141, 10)   // "masterTime"
    },
    "VideoSender",
    "spsReady",
    "",
    "ppsReady",
    "onFrame",
    "MediaFrame",
    "frame",
    "addClient",
    "QHostAddress",
    "addr",
    "port",
    "removeClient",
    "clearClients",
    "setMasterTime",
    "MasterTime*",
    "masterTime"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_VideoSender[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   56,    2, 0x06,    1 /* Public */,
       3,    1,   59,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    1,   62,    2, 0x0a,    5 /* Public */,
       7,    2,   65,    2, 0x0a,    7 /* Public */,
      11,    2,   70,    2, 0x0a,   10 /* Public */,
      12,    0,   75,    2, 0x0a,   13 /* Public */,
      13,    1,   76,    2, 0x0a,   14 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 8, QMetaType::UShort,    9,   10,
    QMetaType::Void, 0x80000000 | 8, QMetaType::UShort,    9,   10,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,

       0        // eod
};

Q_CONSTINIT const QMetaObject VideoSender::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_VideoSender.offsetsAndSizes,
    qt_meta_data_VideoSender,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_VideoSender_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VideoSender, std::true_type>,
        // method 'spsReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        // method 'ppsReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        // method 'onFrame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const MediaFrame &, std::false_type>,
        // method 'addClient'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QHostAddress &, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint16, std::false_type>,
        // method 'removeClient'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QHostAddress &, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint16, std::false_type>,
        // method 'clearClients'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setMasterTime'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<MasterTime *, std::false_type>
    >,
    nullptr
} };

void VideoSender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoSender *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->spsReady((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 1: _t->ppsReady((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 2: _t->onFrame((*reinterpret_cast< std::add_pointer_t<MediaFrame>>(_a[1]))); break;
        case 3: _t->addClient((*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<quint16>>(_a[2]))); break;
        case 4: _t->removeClient((*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<quint16>>(_a[2]))); break;
        case 5: _t->clearClients(); break;
        case 6: _t->setMasterTime((*reinterpret_cast< std::add_pointer_t<MasterTime*>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< MasterTime* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoSender::*)(const QByteArray & );
            if (_t _q_method = &VideoSender::spsReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoSender::*)(const QByteArray & );
            if (_t _q_method = &VideoSender::ppsReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *VideoSender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoSender::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoSender.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VideoSender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void VideoSender::spsReady(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VideoSender::ppsReady(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
