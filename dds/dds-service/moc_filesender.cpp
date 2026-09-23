/****************************************************************************
** Meta object code from reading C++ file 'filesender.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "filesender.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filesender.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_FileSender_t {
    uint offsetsAndSizes[40];
    char stringdata0[11];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[3];
    char stringdata4[17];
    char stringdata5[5];
    char stringdata6[6];
    char stringdata7[8];
    char stringdata8[17];
    char stringdata9[11];
    char stringdata10[9];
    char stringdata11[9];
    char stringdata12[14];
    char stringdata13[6];
    char stringdata14[16];
    char stringdata15[19];
    char stringdata16[6];
    char stringdata17[19];
    char stringdata18[19];
    char stringdata19[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_FileSender_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_FileSender_t qt_meta_stringdata_FileSender = {
    {
        QT_MOC_LITERAL(0, 10),  // "FileSender"
        QT_MOC_LITERAL(11, 15),  // "clientConnected"
        QT_MOC_LITERAL(27, 0),  // ""
        QT_MOC_LITERAL(28, 2),  // "ip"
        QT_MOC_LITERAL(31, 16),  // "transferProgress"
        QT_MOC_LITERAL(48, 4),  // "sent"
        QT_MOC_LITERAL(53, 5),  // "total"
        QT_MOC_LITERAL(59, 7),  // "percent"
        QT_MOC_LITERAL(67, 16),  // "transferFinished"
        QT_MOC_LITERAL(84, 10),  // "transferId"
        QT_MOC_LITERAL(95, 8),  // "senderIp"
        QT_MOC_LITERAL(104, 8),  // "filename"
        QT_MOC_LITERAL(113, 13),  // "transferError"
        QT_MOC_LITERAL(127, 5),  // "error"
        QT_MOC_LITERAL(133, 15),  // "socketConnected"
        QT_MOC_LITERAL(149, 18),  // "socketBytesWritten"
        QT_MOC_LITERAL(168, 5),  // "bytes"
        QT_MOC_LITERAL(174, 18),  // "socketDisconnected"
        QT_MOC_LITERAL(193, 18),  // "calculateChunkSize"
        QT_MOC_LITERAL(212, 13)   // "discoveryTime"
    },
    "FileSender",
    "clientConnected",
    "",
    "ip",
    "transferProgress",
    "sent",
    "total",
    "percent",
    "transferFinished",
    "transferId",
    "senderIp",
    "filename",
    "transferError",
    "error",
    "socketConnected",
    "socketBytesWritten",
    "bytes",
    "socketDisconnected",
    "calculateChunkSize",
    "discoveryTime"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_FileSender[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x06,    1 /* Public */,
       4,    4,   65,    2, 0x06,    3 /* Public */,
       8,    3,   74,    2, 0x06,    8 /* Public */,
      12,    2,   81,    2, 0x06,   12 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      14,    0,   86,    2, 0x08,   15 /* Private */,
      15,    1,   87,    2, 0x08,   16 /* Private */,
      17,    0,   90,    2, 0x08,   18 /* Private */,
      18,    1,   91,    2, 0x08,   19 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::LongLong, QMetaType::LongLong, QMetaType::Int,    3,    5,    6,    7,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QString, QMetaType::QString,    9,   10,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,   13,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   16,
    QMetaType::Void,
    QMetaType::Int, QMetaType::ULongLong,   19,

       0        // eod
};

Q_CONSTINIT const QMetaObject FileSender::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_FileSender.offsetsAndSizes,
    qt_meta_data_FileSender,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_FileSender_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<FileSender, std::true_type>,
        // method 'clientConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'transferProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'transferFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint64, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'transferError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'socketConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'socketBytesWritten'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'socketDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'calculateChunkSize'
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint64, std::false_type>
    >,
    nullptr
} };

void FileSender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FileSender *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->clientConnected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->transferProgress((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4]))); break;
        case 2: _t->transferFinished((*reinterpret_cast< std::add_pointer_t<quint64>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 3: _t->transferError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->socketConnected(); break;
        case 5: _t->socketBytesWritten((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 6: _t->socketDisconnected(); break;
        case 7: { int _r = _t->calculateChunkSize((*reinterpret_cast< std::add_pointer_t<quint64>>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FileSender::*)(QString );
            if (_t _q_method = &FileSender::clientConnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FileSender::*)(QString , qint64 , qint64 , int );
            if (_t _q_method = &FileSender::transferProgress; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FileSender::*)(quint64 , QString , QString );
            if (_t _q_method = &FileSender::transferFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (FileSender::*)(QString , QString );
            if (_t _q_method = &FileSender::transferError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *FileSender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FileSender::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FileSender.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FileSender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void FileSender::clientConnected(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FileSender::transferProgress(QString _t1, qint64 _t2, qint64 _t3, int _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FileSender::transferFinished(quint64 _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FileSender::transferError(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
