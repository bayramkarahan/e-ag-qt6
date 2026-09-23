/****************************************************************************
** Meta object code from reading C++ file 'waylandscreencast.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "capture/waylandscreencast.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'waylandscreencast.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_WaylandScreenCast_t {
    uint offsetsAndSizes[30];
    char stringdata0[18];
    char stringdata1[15];
    char stringdata2[1];
    char stringdata3[16];
    char stringdata4[8];
    char stringdata5[21];
    char stringdata6[3];
    char stringdata7[6];
    char stringdata8[6];
    char stringdata9[8];
    char stringdata10[24];
    char stringdata11[9];
    char stringdata12[8];
    char stringdata13[24];
    char stringdata14[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_WaylandScreenCast_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_WaylandScreenCast_t qt_meta_stringdata_WaylandScreenCast = {
    {
        QT_MOC_LITERAL(0, 17),  // "WaylandScreenCast"
        QT_MOC_LITERAL(18, 14),  // "sessionCreated"
        QT_MOC_LITERAL(33, 0),  // ""
        QT_MOC_LITERAL(34, 15),  // "sourcesSelected"
        QT_MOC_LITERAL(50, 7),  // "started"
        QT_MOC_LITERAL(58, 20),  // "pipeWireRemoteOpened"
        QT_MOC_LITERAL(79, 2),  // "fd"
        QT_MOC_LITERAL(82, 5),  // "ready"
        QT_MOC_LITERAL(88, 5),  // "error"
        QT_MOC_LITERAL(94, 7),  // "message"
        QT_MOC_LITERAL(102, 23),  // "onCreateSessionResponse"
        QT_MOC_LITERAL(126, 8),  // "response"
        QT_MOC_LITERAL(135, 7),  // "results"
        QT_MOC_LITERAL(143, 23),  // "onSelectSourcesResponse"
        QT_MOC_LITERAL(167, 15)   // "onStartResponse"
    },
    "WaylandScreenCast",
    "sessionCreated",
    "",
    "sourcesSelected",
    "started",
    "pipeWireRemoteOpened",
    "fd",
    "ready",
    "error",
    "message",
    "onCreateSessionResponse",
    "response",
    "results",
    "onSelectSourcesResponse",
    "onStartResponse"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_WaylandScreenCast[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x06,    1 /* Public */,
       3,    0,   69,    2, 0x06,    2 /* Public */,
       4,    0,   70,    2, 0x06,    3 /* Public */,
       5,    1,   71,    2, 0x06,    4 /* Public */,
       7,    0,   74,    2, 0x06,    6 /* Public */,
       8,    1,   75,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      10,    2,   78,    2, 0x08,    9 /* Private */,
      13,    2,   83,    2, 0x08,   12 /* Private */,
      14,    2,   88,    2, 0x08,   15 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,

 // slots: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::QVariantMap,   11,   12,
    QMetaType::Void, QMetaType::UInt, QMetaType::QVariantMap,   11,   12,
    QMetaType::Void, QMetaType::UInt, QMetaType::QVariantMap,   11,   12,

       0        // eod
};

Q_CONSTINIT const QMetaObject WaylandScreenCast::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_WaylandScreenCast.offsetsAndSizes,
    qt_meta_data_WaylandScreenCast,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_WaylandScreenCast_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<WaylandScreenCast, std::true_type>,
        // method 'sessionCreated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sourcesSelected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'started'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pipeWireRemoteOpened'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'ready'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'error'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onCreateSessionResponse'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantMap &, std::false_type>,
        // method 'onSelectSourcesResponse'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantMap &, std::false_type>,
        // method 'onStartResponse'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<uint, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantMap &, std::false_type>
    >,
    nullptr
} };

void WaylandScreenCast::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WaylandScreenCast *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sessionCreated(); break;
        case 1: _t->sourcesSelected(); break;
        case 2: _t->started(); break;
        case 3: _t->pipeWireRemoteOpened((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->ready(); break;
        case 5: _t->error((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->onCreateSessionResponse((*reinterpret_cast< std::add_pointer_t<uint>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[2]))); break;
        case 7: _t->onSelectSourcesResponse((*reinterpret_cast< std::add_pointer_t<uint>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[2]))); break;
        case 8: _t->onStartResponse((*reinterpret_cast< std::add_pointer_t<uint>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WaylandScreenCast::*)();
            if (_t _q_method = &WaylandScreenCast::sessionCreated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WaylandScreenCast::*)();
            if (_t _q_method = &WaylandScreenCast::sourcesSelected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (WaylandScreenCast::*)();
            if (_t _q_method = &WaylandScreenCast::started; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (WaylandScreenCast::*)(int );
            if (_t _q_method = &WaylandScreenCast::pipeWireRemoteOpened; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (WaylandScreenCast::*)();
            if (_t _q_method = &WaylandScreenCast::ready; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (WaylandScreenCast::*)(const QString & );
            if (_t _q_method = &WaylandScreenCast::error; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject *WaylandScreenCast::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WaylandScreenCast::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WaylandScreenCast.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WaylandScreenCast::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void WaylandScreenCast::sessionCreated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void WaylandScreenCast::sourcesSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void WaylandScreenCast::started()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void WaylandScreenCast::pipeWireRemoteOpened(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void WaylandScreenCast::ready()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void WaylandScreenCast::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
