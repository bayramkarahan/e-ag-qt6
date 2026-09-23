/****************************************************************************
** Meta object code from reading C++ file 'sessiondescription.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "session/sessiondescription.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sessiondescription.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_SessionDescription_t {
    uint offsetsAndSizes[20];
    char stringdata0[19];
    char stringdata1[7];
    char stringdata2[1];
    char stringdata3[4];
    char stringdata4[7];
    char stringdata5[4];
    char stringdata6[10];
    char stringdata7[16];
    char stringdata8[8];
    char stringdata9[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_SessionDescription_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_SessionDescription_t qt_meta_stringdata_SessionDescription = {
    {
        QT_MOC_LITERAL(0, 18),  // "SessionDescription"
        QT_MOC_LITERAL(19, 6),  // "setSps"
        QT_MOC_LITERAL(26, 0),  // ""
        QT_MOC_LITERAL(27, 3),  // "sps"
        QT_MOC_LITERAL(31, 6),  // "setPps"
        QT_MOC_LITERAL(38, 3),  // "pps"
        QT_MOC_LITERAL(42, 9),  // "createSdp"
        QT_MOC_LITERAL(52, 15),  // "setVideoEnabled"
        QT_MOC_LITERAL(68, 7),  // "enabled"
        QT_MOC_LITERAL(76, 15)   // "setAudioEnabled"
    },
    "SessionDescription",
    "setSps",
    "",
    "sps",
    "setPps",
    "pps",
    "createSdp",
    "setVideoEnabled",
    "enabled",
    "setAudioEnabled"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_SessionDescription[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   44,    2, 0x0a,    1 /* Public */,
       4,    1,   47,    2, 0x0a,    3 /* Public */,
       6,    0,   50,    2, 0x10a,    5 /* Public | MethodIsConst  */,
       7,    1,   51,    2, 0x0a,    6 /* Public */,
       9,    1,   54,    2, 0x0a,    8 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::QString,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,

       0        // eod
};

Q_CONSTINIT const QMetaObject SessionDescription::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_SessionDescription.offsetsAndSizes,
    qt_meta_data_SessionDescription,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_SessionDescription_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SessionDescription, std::true_type>,
        // method 'setSps'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        // method 'setPps'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        // method 'createSdp'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'setVideoEnabled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'setAudioEnabled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void SessionDescription::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SessionDescription *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->setSps((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 1: _t->setPps((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 2: { QString _r = _t->createSdp();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->setVideoEnabled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->setAudioEnabled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *SessionDescription::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SessionDescription::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SessionDescription.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SessionDescription::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
