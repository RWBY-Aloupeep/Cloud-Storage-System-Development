/****************************************************************************
** Meta object code from reading C++ file 'vip.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.10)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../TcpClient/vip.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vip.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.10. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VIP_t {
    QByteArrayData data[10];
    char stringdata0[136];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VIP_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VIP_t qt_meta_stringdata_VIP = {
    {
QT_MOC_LITERAL(0, 0, 3), // "VIP"
QT_MOC_LITERAL(1, 4, 22), // "on_clearBin_pb_clicked"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 25), // "on_recoverfile_pb_clicked"
QT_MOC_LITERAL(4, 54, 8), // "flushBin"
QT_MOC_LITERAL(5, 63, 8), // "enterBin"
QT_MOC_LITERAL(6, 72, 11), // "QModelIndex"
QT_MOC_LITERAL(7, 84, 5), // "index"
QT_MOC_LITERAL(8, 90, 21), // "on_zipfile_pb_clicked"
QT_MOC_LITERAL(9, 112, 23) // "on_unzipfile_pb_clicked"

    },
    "VIP\0on_clearBin_pb_clicked\0\0"
    "on_recoverfile_pb_clicked\0flushBin\0"
    "enterBin\0QModelIndex\0index\0"
    "on_zipfile_pb_clicked\0on_unzipfile_pb_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VIP[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x0a /* Public */,
       4,    0,   46,    2, 0x0a /* Public */,
       5,    1,   47,    2, 0x0a /* Public */,
       8,    0,   50,    2, 0x0a /* Public */,
       9,    0,   51,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VIP::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VIP *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_clearBin_pb_clicked(); break;
        case 1: _t->on_recoverfile_pb_clicked(); break;
        case 2: _t->flushBin(); break;
        case 3: _t->enterBin((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 4: _t->on_zipfile_pb_clicked(); break;
        case 5: _t->on_unzipfile_pb_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VIP::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_VIP.data,
    qt_meta_data_VIP,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VIP::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VIP::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VIP.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int VIP::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
