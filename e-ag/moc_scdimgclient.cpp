/****************************************************************************
** Meta object code from reading C++ file 'scdimgclient.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "scdimgclient.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scdimgclient.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_SCDImgClient_t {
    uint offsetsAndSizes[52];
    char stringdata0[13];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[11];
    char stringdata4[5];
    char stringdata5[5];
    char stringdata6[17];
    char stringdata7[8];
    char stringdata8[12];
    char stringdata9[12];
    char stringdata10[9];
    char stringdata11[7];
    char stringdata12[15];
    char stringdata13[15];
    char stringdata14[17];
    char stringdata15[9];
    char stringdata16[10];
    char stringdata17[13];
    char stringdata18[9];
    char stringdata19[11];
    char stringdata20[12];
    char stringdata21[15];
    char stringdata22[8];
    char stringdata23[29];
    char stringdata24[12];
    char stringdata25[12];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_SCDImgClient_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_SCDImgClient_t qt_meta_stringdata_SCDImgClient = {
    {
        QT_MOC_LITERAL(0, 12),  // "SCDImgClient"
        QT_MOC_LITERAL(13, 15),  // "notifyConnected"
        QT_MOC_LITERAL(29, 0),  // ""
        QT_MOC_LITERAL(30, 10),  // "notifyMess"
        QT_MOC_LITERAL(41, 4),  // "host"
        QT_MOC_LITERAL(46, 4),  // "port"
        QT_MOC_LITERAL(51, 16),  // "notifyDisconnect"
        QT_MOC_LITERAL(68, 7),  // "success"
        QT_MOC_LITERAL(76, 11),  // "notifyError"
        QT_MOC_LITERAL(88, 11),  // "errorString"
        QT_MOC_LITERAL(100, 8),  // "finished"
        QT_MOC_LITERAL(109, 6),  // "errMsg"
        QT_MOC_LITERAL(116, 14),  // "deleteFinished"
        QT_MOC_LITERAL(131, 14),  // "uploadFinished"
        QT_MOC_LITERAL(146, 16),  // "downloadFinished"
        QT_MOC_LITERAL(163, 8),  // "fileName"
        QT_MOC_LITERAL(172, 9),  // "rcvBuffer"
        QT_MOC_LITERAL(182, 12),  // "fileReceived"
        QT_MOC_LITERAL(195, 8),  // "filePath"
        QT_MOC_LITERAL(204, 10),  // "fileSaving"
        QT_MOC_LITERAL(215, 11),  // "onConnected"
        QT_MOC_LITERAL(227, 14),  // "onDisconnected"
        QT_MOC_LITERAL(242, 7),  // "onError"
        QT_MOC_LITERAL(250, 28),  // "QAbstractSocket::SocketError"
        QT_MOC_LITERAL(279, 11),  // "socketError"
        QT_MOC_LITERAL(291, 11)   // "onReadyRead"
    },
    "SCDImgClient",
    "notifyConnected",
    "",
    "notifyMess",
    "host",
    "port",
    "notifyDisconnect",
    "success",
    "notifyError",
    "errorString",
    "finished",
    "errMsg",
    "deleteFinished",
    "uploadFinished",
    "downloadFinished",
    "fileName",
    "rcvBuffer",
    "fileReceived",
    "filePath",
    "fileSaving",
    "onConnected",
    "onDisconnected",
    "onError",
    "QAbstractSocket::SocketError",
    "socketError",
    "onReadyRead"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_SCDImgClient[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    3,   98,    2, 0x06,    1 /* Public */,
       6,    2,  105,    2, 0x06,    5 /* Public */,
       8,    1,  110,    2, 0x06,    8 /* Public */,
      10,    2,  113,    2, 0x06,   10 /* Public */,
      12,    2,  118,    2, 0x06,   13 /* Public */,
      13,    2,  123,    2, 0x06,   16 /* Public */,
      14,    2,  128,    2, 0x06,   19 /* Public */,
      14,    4,  133,    2, 0x06,   22 /* Public */,
      17,    1,  142,    2, 0x06,   27 /* Public */,
      19,    1,  145,    2, 0x06,   29 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      20,    0,  148,    2, 0x0a,   31 /* Public */,
      21,    0,  149,    2, 0x0a,   32 /* Public */,
      22,    1,  150,    2, 0x0a,   33 /* Public */,
      25,    0,  153,    2, 0x0a,   35 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::UShort,    3,    4,    5,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    3,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,   11,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,   11,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,   11,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,   11,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString, QMetaType::QByteArray, QMetaType::QString,    7,   15,   16,   11,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void, QMetaType::QString,   18,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 23,   24,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject SCDImgClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QTcpSocket::staticMetaObject>(),
    qt_meta_stringdata_SCDImgClient.offsetsAndSizes,
    qt_meta_data_SCDImgClient,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_SCDImgClient_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SCDImgClient, std::true_type>,
        // method 'notifyConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<quint16, std::false_type>,
        // method 'notifyDisconnect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'notifyError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'finished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'deleteFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'uploadFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'downloadFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'downloadFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'fileReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'fileSaving'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'onConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QAbstractSocket::SocketError, std::false_type>,
        // method 'onReadyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void SCDImgClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SCDImgClient *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->notifyConnected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<quint16>>(_a[3]))); break;
        case 1: _t->notifyDisconnect((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->notifyError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->finished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->deleteFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->uploadFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 6: _t->downloadFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 7: _t->downloadFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 8: _t->fileReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->fileSaving((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->onConnected(); break;
        case 11: _t->onDisconnected(); break;
        case 12: _t->onError((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        case 13: _t->onReadyRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SCDImgClient::*)(QString , QString , quint16 );
            if (_t _q_method = &SCDImgClient::notifyConnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SCDImgClient::*)(bool , QString );
            if (_t _q_method = &SCDImgClient::notifyDisconnect; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SCDImgClient::*)(QString );
            if (_t _q_method = &SCDImgClient::notifyError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SCDImgClient::*)(bool , QString );
            if (_t _q_method = &SCDImgClient::finished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SCDImgClient::*)(bool , QString );
            if (_t _q_method = &SCDImgClient::deleteFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (SCDImgClient::*)(bool , QString );
            if (_t _q_method = &SCDImgClient::uploadFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (SCDImgClient::*)(bool , QString );
            if (_t _q_method = &SCDImgClient::downloadFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (SCDImgClient::*)(bool , QString , const QByteArray & , QString );
            if (_t _q_method = &SCDImgClient::downloadFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (SCDImgClient::*)(QString );
            if (_t _q_method = &SCDImgClient::fileReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (SCDImgClient::*)(QString );
            if (_t _q_method = &SCDImgClient::fileSaving; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 9;
                return;
            }
        }
    }
}

const QMetaObject *SCDImgClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SCDImgClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SCDImgClient.stringdata0))
        return static_cast<void*>(this);
    return QTcpSocket::qt_metacast(_clname);
}

int SCDImgClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void SCDImgClient::notifyConnected(QString _t1, QString _t2, quint16 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SCDImgClient::notifyDisconnect(bool _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SCDImgClient::notifyError(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SCDImgClient::finished(bool _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SCDImgClient::deleteFinished(bool _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SCDImgClient::uploadFinished(bool _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void SCDImgClient::downloadFinished(bool _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void SCDImgClient::downloadFinished(bool _t1, QString _t2, const QByteArray & _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void SCDImgClient::fileReceived(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void SCDImgClient::fileSaving(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
