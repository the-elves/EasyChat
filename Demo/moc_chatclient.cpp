/****************************************************************************
** Meta object code from reading C++ file 'chatclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "chatclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ChatClient_t {
    QByteArrayData data[19];
    char stringdata[307];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ChatClient_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ChatClient_t qt_meta_stringdata_ChatClient = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 16),
QT_MOC_LITERAL(2, 28, 0),
QT_MOC_LITERAL(3, 29, 3),
QT_MOC_LITERAL(4, 33, 10),
QT_MOC_LITERAL(5, 44, 12),
QT_MOC_LITERAL(6, 57, 9),
QT_MOC_LITERAL(7, 67, 7),
QT_MOC_LITERAL(8, 75, 13),
QT_MOC_LITERAL(9, 89, 16),
QT_MOC_LITERAL(10, 106, 15),
QT_MOC_LITERAL(11, 122, 17),
QT_MOC_LITERAL(12, 140, 22),
QT_MOC_LITERAL(13, 163, 16),
QT_MOC_LITERAL(14, 180, 15),
QT_MOC_LITERAL(15, 196, 25),
QT_MOC_LITERAL(16, 222, 26),
QT_MOC_LITERAL(17, 249, 28),
QT_MOC_LITERAL(18, 278, 27)
    },
    "ChatClient\0sendMsgToUnicast\0\0msg\0"
    "stopTimers\0writeMessage\0readyRead\0"
    "unicast\0heartBeatSend\0heartBeatReceive\0"
    "myNewConnection\0fallbackReadyRead\0"
    "incommingNewConnection\0newHostReadyRead\0"
    "msgFromPrevious\0on_pushButtonSend_clicked\0"
    "on_textEditMsg_textChanged\0"
    "on_pushButtonConnect_clicked\0"
    "on_lineEditName_textChanged\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChatClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06,
       4,    0,   97,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       5,    1,   98,    2, 0x0a,
       6,    0,  101,    2, 0x0a,
       7,    0,  102,    2, 0x0a,
       8,    0,  103,    2, 0x0a,
       9,    0,  104,    2, 0x0a,
      10,    0,  105,    2, 0x0a,
      11,    0,  106,    2, 0x0a,
      12,    0,  107,    2, 0x0a,
      13,    0,  108,    2, 0x0a,
      14,    0,  109,    2, 0x0a,
      15,    0,  110,    2, 0x08,
      16,    0,  111,    2, 0x08,
      17,    0,  112,    2, 0x08,
      18,    0,  113,    2, 0x08,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ChatClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ChatClient *_t = static_cast<ChatClient *>(_o);
        switch (_id) {
        case 0: _t->sendMsgToUnicast((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->stopTimers(); break;
        case 2: _t->writeMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->readyRead(); break;
        case 4: _t->unicast(); break;
        case 5: _t->heartBeatSend(); break;
        case 6: _t->heartBeatReceive(); break;
        case 7: _t->myNewConnection(); break;
        case 8: _t->fallbackReadyRead(); break;
        case 9: _t->incommingNewConnection(); break;
        case 10: _t->newHostReadyRead(); break;
        case 11: _t->msgFromPrevious(); break;
        case 12: _t->on_pushButtonSend_clicked(); break;
        case 13: _t->on_textEditMsg_textChanged(); break;
        case 14: _t->on_pushButtonConnect_clicked(); break;
        case 15: _t->on_lineEditName_textChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ChatClient::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ChatClient::sendMsgToUnicast)) {
                *result = 0;
            }
        }
        {
            typedef void (ChatClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ChatClient::stopTimers)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject ChatClient::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ChatClient.data,
      qt_meta_data_ChatClient,  qt_static_metacall, 0, 0}
};


const QMetaObject *ChatClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ChatClient.stringdata))
        return static_cast<void*>(const_cast< ChatClient*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ChatClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void ChatClient::sendMsgToUnicast(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ChatClient::stopTimers()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
