/****************************************************************************
** Meta object code from reading C++ file 'chatclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Demo/chatclient.h"
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
    QByteArrayData data[12];
    char stringdata[185];
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
QT_MOC_LITERAL(4, 33, 12),
QT_MOC_LITERAL(5, 46, 9),
QT_MOC_LITERAL(6, 56, 7),
QT_MOC_LITERAL(7, 64, 9),
QT_MOC_LITERAL(8, 74, 25),
QT_MOC_LITERAL(9, 100, 26),
QT_MOC_LITERAL(10, 127, 28),
QT_MOC_LITERAL(11, 156, 27)
    },
    "ChatClient\0sendMsgToUnicast\0\0msg\0"
    "writeMessage\0readyRead\0unicast\0heartBeat\0"
    "on_pushButtonSend_clicked\0"
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
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       4,    1,   62,    2, 0x0a,
       5,    0,   65,    2, 0x0a,
       6,    0,   66,    2, 0x0a,
       7,    0,   67,    2, 0x0a,
       8,    0,   68,    2, 0x08,
       9,    0,   69,    2, 0x08,
      10,    0,   70,    2, 0x08,
      11,    0,   71,    2, 0x08,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
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
        case 1: _t->writeMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->readyRead(); break;
        case 3: _t->unicast(); break;
        case 4: _t->heartBeat(); break;
        case 5: _t->on_pushButtonSend_clicked(); break;
        case 6: _t->on_textEditMsg_textChanged(); break;
        case 7: _t->on_pushButtonConnect_clicked(); break;
        case 8: _t->on_lineEditName_textChanged(); break;
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
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ChatClient::sendMsgToUnicast(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
