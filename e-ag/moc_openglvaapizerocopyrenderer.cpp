/****************************************************************************
** Meta object code from reading C++ file 'openglvaapizerocopyrenderer.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "streamplayer/renderer/openglvaapizerocopyrenderer.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'openglvaapizerocopyrenderer.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_OpenGLVaapiZeroCopyRenderer_t {
    uint offsetsAndSizes[14];
    char stringdata0[28];
    char stringdata1[8];
    char stringdata2[1];
    char stringdata3[11];
    char stringdata4[6];
    char stringdata5[14];
    char stringdata6[20];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_OpenGLVaapiZeroCopyRenderer_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_OpenGLVaapiZeroCopyRenderer_t qt_meta_stringdata_OpenGLVaapiZeroCopyRenderer = {
    {
        QT_MOC_LITERAL(0, 27),  // "OpenGLVaapiZeroCopyRenderer"
        QT_MOC_LITERAL(28, 7),  // "onFrame"
        QT_MOC_LITERAL(36, 0),  // ""
        QT_MOC_LITERAL(37, 10),  // "MediaFrame"
        QT_MOC_LITERAL(48, 5),  // "frame"
        QT_MOC_LITERAL(54, 13),  // "importSurface"
        QT_MOC_LITERAL(68, 19)   // "VaapiHardwareFrame*"
    },
    "OpenGLVaapiZeroCopyRenderer",
    "onFrame",
    "",
    "MediaFrame",
    "frame",
    "importSurface",
    "VaapiHardwareFrame*"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_OpenGLVaapiZeroCopyRenderer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   26,    2, 0x0a,    1 /* Public */,
       5,    1,   29,    2, 0x0a,    3 /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Bool, 0x80000000 | 6,    4,

       0        // eod
};

Q_CONSTINIT const QMetaObject OpenGLVaapiZeroCopyRenderer::staticMetaObject = { {
    QMetaObject::SuperData::link<AbstractVideoRenderer::staticMetaObject>(),
    qt_meta_stringdata_OpenGLVaapiZeroCopyRenderer.offsetsAndSizes,
    qt_meta_data_OpenGLVaapiZeroCopyRenderer,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_OpenGLVaapiZeroCopyRenderer_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<OpenGLVaapiZeroCopyRenderer, std::true_type>,
        // method 'onFrame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const MediaFrame &, std::false_type>,
        // method 'importSurface'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<VaapiHardwareFrame *, std::false_type>
    >,
    nullptr
} };

void OpenGLVaapiZeroCopyRenderer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OpenGLVaapiZeroCopyRenderer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onFrame((*reinterpret_cast< std::add_pointer_t<MediaFrame>>(_a[1]))); break;
        case 1: { bool _r = _t->importSurface((*reinterpret_cast< std::add_pointer_t<VaapiHardwareFrame*>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject *OpenGLVaapiZeroCopyRenderer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OpenGLVaapiZeroCopyRenderer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OpenGLVaapiZeroCopyRenderer.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QOpenGLFunctions_3_3_Core"))
        return static_cast< QOpenGLFunctions_3_3_Core*>(this);
    return AbstractVideoRenderer::qt_metacast(_clname);
}

int OpenGLVaapiZeroCopyRenderer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractVideoRenderer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
