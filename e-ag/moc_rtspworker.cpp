/****************************************************************************
** Meta object code from reading C++ file 'rtspworker.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "streamplayer/player/rtspworker.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rtspworker.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_RtspWorker_t {
    uint offsetsAndSizes[34];
    char stringdata0[11];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[14];
    char stringdata5[6];
    char stringdata6[12];
    char stringdata7[4];
    char stringdata8[4];
    char stringdata9[7];
    char stringdata10[12];
    char stringdata11[7];
    char stringdata12[8];
    char stringdata13[5];
    char stringdata14[19];
    char stringdata15[9];
    char stringdata16[19];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_RtspWorker_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_RtspWorker_t qt_meta_stringdata_RtspWorker = {
    {
        QT_MOC_LITERAL(0, 10),  // "RtspWorker"
        QT_MOC_LITERAL(11, 9),  // "connected"
        QT_MOC_LITERAL(21, 0),  // ""
        QT_MOC_LITERAL(22, 12),  // "disconnected"
        QT_MOC_LITERAL(35, 13),  // "errorOccurred"
        QT_MOC_LITERAL(49, 5),  // "error"
        QT_MOC_LITERAL(55, 11),  // "videoPacket"
        QT_MOC_LITERAL(67, 3),  // "nal"
        QT_MOC_LITERAL(71, 3),  // "pts"
        QT_MOC_LITERAL(75, 6),  // "marker"
        QT_MOC_LITERAL(82, 11),  // "audioPacket"
        QT_MOC_LITERAL(94, 6),  // "packet"
        QT_MOC_LITERAL(101, 7),  // "process"
        QT_MOC_LITERAL(109, 4),  // "stop"
        QT_MOC_LITERAL(114, 18),  // "processVideoPacket"
        QT_MOC_LITERAL(133, 8),  // "AVPacket"
        QT_MOC_LITERAL(142, 18)   // "processAudioPacket"
    },
    "RtspWorker",
    "connected",
    "",
    "disconnected",
    "errorOccurred",
    "error",
    "videoPacket",
    "nal",
    "pts",
    "marker",
    "audioPacket",
    "packet",
    "process",
    "stop",
    "processVideoPacket",
    "AVPacket",
    "processAudioPacket"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_RtspWorker[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x06,    1 /* Public */,
       3,    0,   69,    2, 0x06,    2 /* Public */,
       4,    1,   70,    2, 0x06,    3 /* Public */,
       6,    3,   73,    2, 0x06,    5 /* Public */,
      10,    2,   80,    2, 0x06,    9 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      12,    0,   85,    2, 0x0a,   12 /* Public */,
      13,    0,   86,    2, 0x0a,   13 /* Public */,
      14,    1,   87,    2, 0x08,   14 /* Private */,
      16,    1,   90,    2, 0x08,   16 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::LongLong, QMetaType::Bool,    7,    8,    9,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::LongLong,   11,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,   11,
    QMetaType::Void, 0x80000000 | 15,   11,

       0        // eod
};

Q_CONSTINIT const QMetaObject RtspWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_RtspWorker.offsetsAndSizes,
    qt_meta_data_RtspWorker,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_RtspWorker_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RtspWorker, std::true_type>,
        // method 'connected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'disconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'errorOccurred'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'videoPacket'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'audioPacket'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'process'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'processVideoPacket'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const AVPacket &, std::false_type>,
        // method 'processAudioPacket'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const AVPacket &, std::false_type>
    >,
    nullptr
} };

void RtspWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RtspWorker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->errorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->videoPacket((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        case 4: _t->audioPacket((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[2]))); break;
        case 5: _t->process(); break;
        case 6: _t->stop(); break;
        case 7: _t->processVideoPacket((*reinterpret_cast< std::add_pointer_t<AVPacket>>(_a[1]))); break;
        case 8: _t->processAudioPacket((*reinterpret_cast< std::add_pointer_t<AVPacket>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RtspWorker::*)();
            if (_t _q_method = &RtspWorker::connected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RtspWorker::*)();
            if (_t _q_method = &RtspWorker::disconnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (RtspWorker::*)(const QString & );
            if (_t _q_method = &RtspWorker::errorOccurred; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (RtspWorker::*)(const QByteArray & , qint64 , bool );
            if (_t _q_method = &RtspWorker::videoPacket; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (RtspWorker::*)(const QByteArray & , qint64 );
            if (_t _q_method = &RtspWorker::audioPacket; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject *RtspWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RtspWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RtspWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RtspWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void RtspWorker::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void RtspWorker::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void RtspWorker::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RtspWorker::videoPacket(const QByteArray & _t1, qint64 _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void RtspWorker::audioPacket(const QByteArray & _t1, qint64 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
