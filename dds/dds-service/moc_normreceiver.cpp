/****************************************************************************
** Meta object code from reading C++ file 'normreceiver.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "normreceiver.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'normreceiver.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_NormReceiver_t {
    uint offsetsAndSizes[30];
    char stringdata0[13];
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
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_NormReceiver_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_NormReceiver_t qt_meta_stringdata_NormReceiver = {
    {
        QT_MOC_LITERAL(0, 12),  // "NormReceiver"
        QT_MOC_LITERAL(13, 8),  // "progress"
        QT_MOC_LITERAL(22, 0),  // ""
        QT_MOC_LITERAL(23, 8),  // "fileName"
        QT_MOC_LITERAL(32, 7),  // "current"
        QT_MOC_LITERAL(40, 5),  // "total"
        QT_MOC_LITERAL(46, 7),  // "percent"
        QT_MOC_LITERAL(54, 12),  // "fileReceived"
        QT_MOC_LITERAL(67, 8),  // "filePath"
        QT_MOC_LITERAL(76, 10),  // "transferId"
        QT_MOC_LITERAL(87, 2),  // "ip"
        QT_MOC_LITERAL(90, 12),  // "senderActive"
        QT_MOC_LITERAL(103, 14),  // "senderInactive"
        QT_MOC_LITERAL(118, 5),  // "error"
        QT_MOC_LITERAL(124, 4)   // "text"
    },
    "NormReceiver",
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
    "text"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_NormReceiver[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    4,   44,    2, 0x06,    1 /* Public */,
       7,    3,   53,    2, 0x06,    6 /* Public */,
      11,    0,   60,    2, 0x06,   10 /* Public */,
      12,    0,   61,    2, 0x06,   11 /* Public */,
      13,    1,   62,    2, 0x06,   12 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::LongLong, QMetaType::LongLong, QMetaType::Double,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::ULongLong, QMetaType::QString,    8,    9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,

       0        // eod
};

Q_CONSTINIT const QMetaObject NormReceiver::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NormReceiver.offsetsAndSizes,
    qt_meta_data_NormReceiver,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_NormReceiver_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<NormReceiver, std::true_type>,
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
        QtPrivate::TypeAndForceComplete<QString, std::false_type>
    >,
    nullptr
} };

void NormReceiver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NormReceiver *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->progress((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4]))); break;
        case 1: _t->fileReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<quint64>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 2: _t->senderActive(); break;
        case 3: _t->senderInactive(); break;
        case 4: _t->error((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NormReceiver::*)(QString , qint64 , qint64 , double );
            if (_t _q_method = &NormReceiver::progress; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NormReceiver::*)(QString , quint64 , QString );
            if (_t _q_method = &NormReceiver::fileReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NormReceiver::*)();
            if (_t _q_method = &NormReceiver::senderActive; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NormReceiver::*)();
            if (_t _q_method = &NormReceiver::senderInactive; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NormReceiver::*)(QString );
            if (_t _q_method = &NormReceiver::error; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject *NormReceiver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NormReceiver::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NormReceiver.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NormReceiver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void NormReceiver::progress(QString _t1, qint64 _t2, qint64 _t3, double _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NormReceiver::fileReceived(QString _t1, quint64 _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NormReceiver::senderActive()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void NormReceiver::senderInactive()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void NormReceiver::error(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
