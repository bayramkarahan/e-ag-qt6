/****************************************************************************
** Meta object code from reading C++ file 'monitorclient.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "monitorclient.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'monitorclient.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_MonitorClient_t {
    uint offsetsAndSizes[108];
    char stringdata0[14];
    char stringdata1[20];
    char stringdata2[1];
    char stringdata3[24];
    char stringdata4[12];
    char stringdata5[11];
    char stringdata6[5];
    char stringdata7[8];
    char stringdata8[7];
    char stringdata9[11];
    char stringdata10[13];
    char stringdata11[15];
    char stringdata12[3];
    char stringdata13[18];
    char stringdata14[30];
    char stringdata15[2];
    char stringdata16[13];
    char stringdata17[7];
    char stringdata18[18];
    char stringdata19[24];
    char stringdata20[17];
    char stringdata21[16];
    char stringdata22[20];
    char stringdata23[19];
    char stringdata24[23];
    char stringdata25[18];
    char stringdata26[13];
    char stringdata27[17];
    char stringdata28[9];
    char stringdata29[19];
    char stringdata30[6];
    char stringdata31[11];
    char stringdata32[25];
    char stringdata33[23];
    char stringdata34[25];
    char stringdata35[21];
    char stringdata36[15];
    char stringdata37[15];
    char stringdata38[8];
    char stringdata39[9];
    char stringdata40[12];
    char stringdata41[13];
    char stringdata42[8];
    char stringdata43[23];
    char stringdata44[5];
    char stringdata45[21];
    char stringdata46[24];
    char stringdata47[15];
    char stringdata48[9];
    char stringdata49[15];
    char stringdata50[15];
    char stringdata51[17];
    char stringdata52[9];
    char stringdata53[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MonitorClient_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MonitorClient_t qt_meta_stringdata_MonitorClient = {
    {
        QT_MOC_LITERAL(0, 13),  // "MonitorClient"
        QT_MOC_LITERAL(14, 19),  // "updateSelectClients"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 23),  // "sendFileTransferAllDone"
        QT_MOC_LITERAL(59, 11),  // "averageTime"
        QT_MOC_LITERAL(71, 10),  // "updatePath"
        QT_MOC_LITERAL(82, 4),  // "path"
        QT_MOC_LITERAL(87, 7),  // "getFile"
        QT_MOC_LITERAL(95, 6),  // "myfile"
        QT_MOC_LITERAL(102, 10),  // "SourceType"
        QT_MOC_LITERAL(113, 12),  // "mySourceType"
        QT_MOC_LITERAL(126, 14),  // "sendRoleStatus"
        QT_MOC_LITERAL(141, 2),  // "ip"
        QT_MOC_LITERAL(144, 17),  // "sendRoleStatusAll"
        QT_MOC_LITERAL(162, 29),  // "receivedMonitorDiscoveryReply"
        QT_MOC_LITERAL(192, 1),  // "d"
        QT_MOC_LITERAL(194, 12),  // "QHostAddress"
        QT_MOC_LITERAL(207, 6),  // "sender"
        QT_MOC_LITERAL(214, 17),  // "receivedClientAdd"
        QT_MOC_LITERAL(232, 23),  // "receivedMonitorProgress"
        QT_MOC_LITERAL(256, 16),  // "receivedDebStart"
        QT_MOC_LITERAL(273, 15),  // "receivedDebDone"
        QT_MOC_LITERAL(289, 19),  // "receivedScriptStart"
        QT_MOC_LITERAL(309, 18),  // "receivedScriptDone"
        QT_MOC_LITERAL(328, 22),  // "isUserSpecialDirectory"
        QT_MOC_LITERAL(351, 17),  // "createListeWidget"
        QT_MOC_LITERAL(369, 12),  // "QScrollArea*"
        QT_MOC_LITERAL(382, 16),  // "createPathWidget"
        QT_MOC_LITERAL(399, 8),  // "QWidget*"
        QT_MOC_LITERAL(408, 18),  // "createButtonWidget"
        QT_MOC_LITERAL(427, 5),  // "about"
        QT_MOC_LITERAL(433, 10),  // "updConnect"
        QT_MOC_LITERAL(444, 24),  // "onDiscoveryButtonClicked"
        QT_MOC_LITERAL(469, 22),  // "selectAllButtonClicked"
        QT_MOC_LITERAL(492, 24),  // "unSelectAllButtonClicked"
        QT_MOC_LITERAL(517, 20),  // "getFileButtonClicked"
        QT_MOC_LITERAL(538, 14),  // "SourceLocation"
        QT_MOC_LITERAL(553, 14),  // "sourceLocation"
        QT_MOC_LITERAL(568, 7),  // "getRole"
        QT_MOC_LITERAL(576, 8),  // "RoleInfo"
        QT_MOC_LITERAL(585, 11),  // "sortClients"
        QT_MOC_LITERAL(597, 12),  // "createHeader"
        QT_MOC_LITERAL(610, 7),  // "QFrame*"
        QT_MOC_LITERAL(618, 22),  // "getDesktopPathFromHome"
        QT_MOC_LITERAL(641, 4),  // "home"
        QT_MOC_LITERAL(646, 20),  // "sendMonitorDiscovery"
        QT_MOC_LITERAL(667, 23),  // "processPendingDatagrams"
        QT_MOC_LITERAL(691, 14),  // "sendSenderData"
        QT_MOC_LITERAL(706, 8),  // "destPath"
        QT_MOC_LITERAL(715, 14),  // "TargetLocation"
        QT_MOC_LITERAL(730, 14),  // "targetLocation"
        QT_MOC_LITERAL(745, 16),  // "sendReceiverData"
        QT_MOC_LITERAL(762, 8),  // "fileName"
        QT_MOC_LITERAL(771, 10)   // "sourceType"
    },
    "MonitorClient",
    "updateSelectClients",
    "",
    "sendFileTransferAllDone",
    "averageTime",
    "updatePath",
    "path",
    "getFile",
    "myfile",
    "SourceType",
    "mySourceType",
    "sendRoleStatus",
    "ip",
    "sendRoleStatusAll",
    "receivedMonitorDiscoveryReply",
    "d",
    "QHostAddress",
    "sender",
    "receivedClientAdd",
    "receivedMonitorProgress",
    "receivedDebStart",
    "receivedDebDone",
    "receivedScriptStart",
    "receivedScriptDone",
    "isUserSpecialDirectory",
    "createListeWidget",
    "QScrollArea*",
    "createPathWidget",
    "QWidget*",
    "createButtonWidget",
    "about",
    "updConnect",
    "onDiscoveryButtonClicked",
    "selectAllButtonClicked",
    "unSelectAllButtonClicked",
    "getFileButtonClicked",
    "SourceLocation",
    "sourceLocation",
    "getRole",
    "RoleInfo",
    "sortClients",
    "createHeader",
    "QFrame*",
    "getDesktopPathFromHome",
    "home",
    "sendMonitorDiscovery",
    "processPendingDatagrams",
    "sendSenderData",
    "destPath",
    "TargetLocation",
    "targetLocation",
    "sendReceiverData",
    "fileName",
    "sourceType"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MonitorClient[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  200,    2, 0x06,    1 /* Public */,
       3,    1,  201,    2, 0x06,    2 /* Public */,
       5,    1,  204,    2, 0x06,    4 /* Public */,
       7,    2,  207,    2, 0x06,    6 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      11,    1,  212,    2, 0x0a,    9 /* Public */,
      13,    0,  215,    2, 0x0a,   11 /* Public */,
      14,    2,  216,    2, 0x0a,   12 /* Public */,
      18,    2,  221,    2, 0x0a,   15 /* Public */,
      19,    2,  226,    2, 0x0a,   18 /* Public */,
      20,    2,  231,    2, 0x0a,   21 /* Public */,
      21,    2,  236,    2, 0x0a,   24 /* Public */,
      22,    2,  241,    2, 0x0a,   27 /* Public */,
      23,    2,  246,    2, 0x0a,   30 /* Public */,
      24,    1,  251,    2, 0x08,   33 /* Private */,
      25,    0,  254,    2, 0x08,   35 /* Private */,
      27,    0,  255,    2, 0x08,   36 /* Private */,
      29,    0,  256,    2, 0x08,   37 /* Private */,
      30,    0,  257,    2, 0x08,   38 /* Private */,
      31,    0,  258,    2, 0x08,   39 /* Private */,
      32,    0,  259,    2, 0x08,   40 /* Private */,
      33,    0,  260,    2, 0x08,   41 /* Private */,
      34,    0,  261,    2, 0x08,   42 /* Private */,
      35,    1,  262,    2, 0x08,   43 /* Private */,
      38,    0,  265,    2, 0x08,   45 /* Private */,
      40,    0,  266,    2, 0x08,   46 /* Private */,
      41,    0,  267,    2, 0x08,   47 /* Private */,
      43,    1,  268,    2, 0x08,   48 /* Private */,
      45,    0,  271,    2, 0x08,   50 /* Private */,
      46,    0,  272,    2, 0x08,   51 /* Private */,
      47,    2,  273,    2, 0x08,   52 /* Private */,
      51,    4,  278,    2, 0x08,   55 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 9,    8,   10,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 16,   15,   17,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 16,   15,   17,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 16,   15,   17,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 16,   15,   17,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 16,   15,   17,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 16,   15,   17,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 16,   15,   17,
    QMetaType::Bool, QMetaType::QString,    6,
    0x80000000 | 26,
    0x80000000 | 28,
    0x80000000 | 28,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 36,   37,
    0x80000000 | 39,
    QMetaType::Void,
    0x80000000 | 42,
    QMetaType::QString, QMetaType::QString,   44,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 49,   48,   50,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 9, 0x80000000 | 36, 0x80000000 | 49,   52,   53,   37,   50,

       0        // eod
};

Q_CONSTINIT const QMetaObject MonitorClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_MonitorClient.offsetsAndSizes,
    qt_meta_data_MonitorClient,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_MonitorClient_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MonitorClient, std::true_type>,
        // method 'updateSelectClients'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendFileTransferAllDone'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'updatePath'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'getFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<SourceType, std::false_type>,
        // method 'sendRoleStatus'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'sendRoleStatusAll'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'receivedMonitorDiscoveryReply'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'receivedClientAdd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'receivedMonitorProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'receivedDebStart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'receivedDebDone'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'receivedScriptStart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'receivedScriptDone'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>,
        // method 'isUserSpecialDirectory'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'createListeWidget'
        QtPrivate::TypeAndForceComplete<QScrollArea *, std::false_type>,
        // method 'createPathWidget'
        QtPrivate::TypeAndForceComplete<QWidget *, std::false_type>,
        // method 'createButtonWidget'
        QtPrivate::TypeAndForceComplete<QWidget *, std::false_type>,
        // method 'about'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updConnect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDiscoveryButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'selectAllButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'unSelectAllButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'getFileButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<SourceLocation, std::false_type>,
        // method 'getRole'
        QtPrivate::TypeAndForceComplete<RoleInfo, std::false_type>,
        // method 'sortClients'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createHeader'
        QtPrivate::TypeAndForceComplete<QFrame *, std::false_type>,
        // method 'getDesktopPathFromHome'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'sendMonitorDiscovery'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'processPendingDatagrams'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendSenderData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<TargetLocation, std::false_type>,
        // method 'sendReceiverData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<SourceType, std::false_type>,
        QtPrivate::TypeAndForceComplete<SourceLocation, std::false_type>,
        QtPrivate::TypeAndForceComplete<TargetLocation, std::false_type>
    >,
    nullptr
} };

void MonitorClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MonitorClient *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateSelectClients(); break;
        case 1: _t->sendFileTransferAllDone((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 2: _t->updatePath((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->getFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<SourceType>>(_a[2]))); break;
        case 4: _t->sendRoleStatus((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->sendRoleStatusAll(); break;
        case 6: _t->receivedMonitorDiscoveryReply((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 7: _t->receivedClientAdd((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 8: _t->receivedMonitorProgress((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 9: _t->receivedDebStart((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 10: _t->receivedDebDone((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 11: _t->receivedScriptStart((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 12: _t->receivedScriptDone((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 13: { bool _r = _t->isUserSpecialDirectory((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 14: { QScrollArea* _r = _t->createListeWidget();
            if (_a[0]) *reinterpret_cast< QScrollArea**>(_a[0]) = std::move(_r); }  break;
        case 15: { QWidget* _r = _t->createPathWidget();
            if (_a[0]) *reinterpret_cast< QWidget**>(_a[0]) = std::move(_r); }  break;
        case 16: { QWidget* _r = _t->createButtonWidget();
            if (_a[0]) *reinterpret_cast< QWidget**>(_a[0]) = std::move(_r); }  break;
        case 17: _t->about(); break;
        case 18: _t->updConnect(); break;
        case 19: _t->onDiscoveryButtonClicked(); break;
        case 20: _t->selectAllButtonClicked(); break;
        case 21: _t->unSelectAllButtonClicked(); break;
        case 22: _t->getFileButtonClicked((*reinterpret_cast< std::add_pointer_t<SourceLocation>>(_a[1]))); break;
        case 23: { RoleInfo _r = _t->getRole();
            if (_a[0]) *reinterpret_cast< RoleInfo*>(_a[0]) = std::move(_r); }  break;
        case 24: _t->sortClients(); break;
        case 25: { QFrame* _r = _t->createHeader();
            if (_a[0]) *reinterpret_cast< QFrame**>(_a[0]) = std::move(_r); }  break;
        case 26: { QString _r = _t->getDesktopPathFromHome((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 27: _t->sendMonitorDiscovery(); break;
        case 28: _t->processPendingDatagrams(); break;
        case 29: _t->sendSenderData((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<TargetLocation>>(_a[2]))); break;
        case 30: _t->sendReceiverData((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<SourceType>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<SourceLocation>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<TargetLocation>>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MonitorClient::*)();
            if (_t _q_method = &MonitorClient::updateSelectClients; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MonitorClient::*)(qint64 );
            if (_t _q_method = &MonitorClient::sendFileTransferAllDone; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MonitorClient::*)(QString );
            if (_t _q_method = &MonitorClient::updatePath; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MonitorClient::*)(QString , SourceType );
            if (_t _q_method = &MonitorClient::getFile; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *MonitorClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MonitorClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MonitorClient.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int MonitorClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 31)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 31;
    }
    return _id;
}

// SIGNAL 0
void MonitorClient::updateSelectClients()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MonitorClient::sendFileTransferAllDone(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MonitorClient::updatePath(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MonitorClient::getFile(QString _t1, SourceType _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
