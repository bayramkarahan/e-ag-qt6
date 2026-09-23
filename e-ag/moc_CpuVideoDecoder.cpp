/****************************************************************************
** Meta object code from reading C++ file 'CpuVideoDecoder.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "streamplayer/decoder/CpuVideoDecoder.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CpuVideoDecoder.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CpuVideoDecoder_t {
    uint offsetsAndSizes[12];
    char stringdata0[16];
    char stringdata1[7];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[4];
    char stringdata5[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CpuVideoDecoder_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CpuVideoDecoder_t qt_meta_stringdata_CpuVideoDecoder = {
    {
        QT_MOC_LITERAL(0, 15),  // "CpuVideoDecoder"
        QT_MOC_LITERAL(16, 6),  // "decode"
        QT_MOC_LITERAL(23, 0),  // ""
        QT_MOC_LITERAL(24, 6),  // "packet"
        QT_MOC_LITERAL(31, 3),  // "pts"
        QT_MOC_LITERAL(35, 6)   // "marker"
    },
    "CpuVideoDecoder",
    "decode",
    "",
    "packet",
    "pts",
    "marker"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CpuVideoDecoder[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    3,   20,    2, 0x0a,    1 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray, QMetaType::LongLong, QMetaType::Bool,    3,    4,    5,

       0        // eod
};

Q_CONSTINIT const QMetaObject CpuVideoDecoder::staticMetaObject = { {
    QMetaObject::SuperData::link<AbstractVideoDecoder::staticMetaObject>(),
    qt_meta_stringdata_CpuVideoDecoder.offsetsAndSizes,
    qt_meta_data_CpuVideoDecoder,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CpuVideoDecoder_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CpuVideoDecoder, std::true_type>,
        // method 'decode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void CpuVideoDecoder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CpuVideoDecoder *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->decode((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qint64>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject *CpuVideoDecoder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CpuVideoDecoder::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CpuVideoDecoder.stringdata0))
        return static_cast<void*>(this);
    return AbstractVideoDecoder::qt_metacast(_clname);
}

int CpuVideoDecoder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractVideoDecoder::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
