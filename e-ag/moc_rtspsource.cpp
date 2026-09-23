/****************************************************************************
** Meta object code from reading C++ file 'rtspsource.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "streamplayer/player/rtspsource.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rtspsource.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_RtspSource_t {
    uint offsetsAndSizes[28];
    char stringdata0[11];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[11];
    char stringdata4[10];
    char stringdata5[13];
    char stringdata6[14];
    char stringdata7[6];
    char stringdata8[12];
    char stringdata9[4];
    char stringdata10[4];
    char stringdata11[7];
    char stringdata12[12];
    char stringdata13[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_RtspSource_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_RtspSource_t qt_meta_stringdata_RtspSource = {
    {
        QT_MOC_LITERAL(0, 10),  // "RtspSource"
        QT_MOC_LITERAL(11, 11),  // "startWorker"
        QT_MOC_LITERAL(23, 0),  // ""
        QT_MOC_LITERAL(24, 10),  // "stopWorker"
        QT_MOC_LITERAL(35, 9),  // "connected"
        QT_MOC_LITERAL(45, 12),  // "disconnected"
        QT_MOC_LITERAL(58, 13),  // "errorOccurred"
        QT_MOC_LITERAL(72, 5),  // "error"
        QT_MOC_LITERAL(78, 11),  // "videoPacket"
        QT_MOC_LITERAL(90, 3),  // "nal"
        QT_MOC_LITERAL(94, 3),  // "pts"
        QT_MOC_LITERAL(98, 6),  // "marker"
        QT_MOC_LITERAL(105, 11),  // "audioPacket"
        QT_MOC_LITERAL(117, 6)   // "packet"
    },
    "RtspSource",
    "startWorker",
    "",
    "stopWorker",
    "connected",
    "disconnected",
    "errorOccurred",
    "error",
    "videoPacket",
    "nal",
    "pts",
    "marker",
    "audioPacket",
    "packet"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_RtspSource[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   56,    2, 0x06,    1 /* Public */,
       3,    0,   57,    2, 0x06,    2 /* Public */,
       4,    0,   58,    2, 0x06,    3 /* Public */,
       5,    0,   59,    2, 0x06,    4 /* Public */,
       6,    1,   60,    2, 0x06,    5 /* Public */,
       8,    3,   63,    2, 0x06,    7 /* Public */,
      12,    2,   70,    2, 0x06,   11 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::LongLong, QMetaType::Bool,    9,   10,   11,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::LongLong,   13,   10,

       0        // eod
};

Q_CONSTINIT const QMetaObject RtspSource::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_RtspSource.offsetsAndSizes,
    qt_meta_data_RtspSource,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_RtspSource_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RtspSource, std::true_type>,
        // method 'startWorker'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopWorker'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
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
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>
    >,
    nullptr
} };

void RtspSource::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RtspSource *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->startWorker(); break;
        case 1: _t->stopWorker(); break;
        case 2: _t->connected(); break;
        case 3: _t->disconnected(); break;
        case 4: _t->errorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->videoPacket((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        case 6: _t->audioPacket((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RtspSource::*)();
            if (_t _q_method = &RtspSource::startWorker; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RtspSource::*)();
            if (_t _q_method = &RtspSource::stopWorker; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (RtspSource::*)();
            if (_t _q_method = &RtspSource::connected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (RtspSource::*)();
            if (_t _q_method = &RtspSource::disconnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (RtspSource::*)(const QString & );
            if (_t _q_method = &RtspSource::errorOccurred; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (RtspSource::*)(const QByteArray & , qint64 , bool );
            if (_t _q_method = &RtspSource::videoPacket; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (RtspSource::*)(const QByteArray & , qint64 );
            if (_t _q_method = &RtspSource::audioPacket; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject *RtspSource::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RtspSource::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RtspSource.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RtspSource::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void RtspSource::startWorker()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void RtspSource::stopWorker()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void RtspSource::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void RtspSource::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void RtspSource::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void RtspSource::videoPacket(const QByteArray & _t1, qint64 _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void RtspSource::audioPacket(const QByteArray & _t1, qint64 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
