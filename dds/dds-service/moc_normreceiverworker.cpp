/****************************************************************************
** Meta object code from reading C++ file 'normreceiverworker.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "normreceiverworker.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'normreceiverworker.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_NormReceiverWorker_t {
    uint offsetsAndSizes[44];
    char stringdata0[19];
    char stringdata1[9];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[8];
    char stringdata5[6];
    char stringdata6[8];
    char stringdata7[13];
    char stringdata8[9];
    char stringdata9[11];
    char stringdata10[3];
    char stringdata11[13];
    char stringdata12[15];
    char stringdata13[6];
    char stringdata14[5];
    char stringdata15[13];
    char stringdata16[14];
    char stringdata17[9];
    char stringdata18[12];
    char stringdata19[5];
    char stringdata20[5];
    char stringdata21[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_NormReceiverWorker_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_NormReceiverWorker_t qt_meta_stringdata_NormReceiverWorker = {
    {
        QT_MOC_LITERAL(0, 18),  // "NormReceiverWorker"
        QT_MOC_LITERAL(19, 8),  // "progress"
        QT_MOC_LITERAL(28, 0),  // ""
        QT_MOC_LITERAL(29, 8),  // "fileName"
        QT_MOC_LITERAL(38, 7),  // "current"
        QT_MOC_LITERAL(46, 5),  // "total"
        QT_MOC_LITERAL(52, 7),  // "percent"
        QT_MOC_LITERAL(60, 12),  // "fileReceived"
        QT_MOC_LITERAL(73, 8),  // "filePath"
        QT_MOC_LITERAL(82, 10),  // "transferId"
        QT_MOC_LITERAL(93, 2),  // "ip"
        QT_MOC_LITERAL(96, 12),  // "senderActive"
        QT_MOC_LITERAL(109, 14),  // "senderInactive"
        QT_MOC_LITERAL(124, 5),  // "error"
        QT_MOC_LITERAL(130, 4),  // "text"
        QT_MOC_LITERAL(135, 12),  // "stopTransfer"
        QT_MOC_LITERAL(148, 13),  // "startReceiver"
        QT_MOC_LITERAL(162, 8),  // "cacheDir"
        QT_MOC_LITERAL(171, 11),  // "multicastIp"
        QT_MOC_LITERAL(183, 4),  // "port"
        QT_MOC_LITERAL(188, 4),  // "stop"
        QT_MOC_LITERAL(193, 17)   // "detectNetworkType"
    },
    "NormReceiverWorker",
    "progress",
    "",
    "fileName",
    "current",
    "total",
    "percent",
    "fileReceived",
    "filePath",
    "transferId",
    "ip",
    "senderActive",
    "senderInactive",
    "error",
    "text",
    "stopTransfer",
    "startReceiver",
    "cacheDir",
    "multicastIp",
    "port",
    "stop",
    "detectNetworkType"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_NormReceiverWorker[] = {

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
       1,    4,   68,    2, 0x06,    1 /* Public */,
       7,    3,   77,    2, 0x06,    6 /* Public */,
      11,    0,   84,    2, 0x06,   10 /* Public */,
      12,    0,   85,    2, 0x06,   11 /* Public */,
      13,    1,   86,    2, 0x06,   12 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      15,    1,   89,    2, 0x0a,   14 /* Public */,
      16,    3,   92,    2, 0x0a,   16 /* Public */,
      20,    0,   99,    2, 0x0a,   20 /* Public */,
      21,    0,  100,    2, 0x0a,   21 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::LongLong, QMetaType::LongLong, QMetaType::Double,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::ULongLong, QMetaType::QString,    8,    9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,

 // slots: parameters
    QMetaType::Void, QMetaType::ULongLong,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::UShort,   17,   18,   19,
    QMetaType::Void,
    QMetaType::QString,

       0        // eod
};

Q_CONSTINIT const QMetaObject NormReceiverWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NormReceiverWorker.offsetsAndSizes,
    qt_meta_data_NormReceiverWorker,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_NormReceiverWorker_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<NormReceiverWorker, std::true_type>,
        // method 'progress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'fileReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint64, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'senderActive'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'senderInactive'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'error'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'stopTransfer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint64, std::false_type>,
        // method 'startReceiver'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint16, std::false_type>,
        // method 'stop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'detectNetworkType'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>
    >,
    nullptr
} };

void NormReceiverWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NormReceiverWorker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->progress((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4]))); break;
        case 1: _t->fileReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<quint64>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 2: _t->senderActive(); break;
        case 3: _t->senderInactive(); break;
        case 4: _t->error((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->stopTransfer((*reinterpret_cast< std::add_pointer_t<quint64>>(_a[1]))); break;
        case 6: _t->startReceiver((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<quint16>>(_a[3]))); break;
        case 7: _t->stop(); break;
        case 8: { QString _r = _t->detectNetworkType();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NormReceiverWorker::*)(QString , qint64 , qint64 , double );
            if (_t _q_method = &NormReceiverWorker::progress; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NormReceiverWorker::*)(QString , quint64 , QString );
            if (_t _q_method = &NormReceiverWorker::fileReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NormReceiverWorker::*)();
            if (_t _q_method = &NormReceiverWorker::senderActive; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NormReceiverWorker::*)();
            if (_t _q_method = &NormReceiverWorker::senderInactive; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NormReceiverWorker::*)(QString );
            if (_t _q_method = &NormReceiverWorker::error; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject *NormReceiverWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NormReceiverWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NormReceiverWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NormReceiverWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void NormReceiverWorker::progress(QString _t1, qint64 _t2, qint64 _t3, double _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NormReceiverWorker::fileReceived(QString _t1, quint64 _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NormReceiverWorker::senderActive()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void NormReceiverWorker::senderInactive()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void NormReceiverWorker::error(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
