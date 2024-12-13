/****************************************************************************
** Meta object code from reading C++ file 'book.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.10)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../TcpClient/book.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'book.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.10. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Book_t {
    QByteArrayData data[25];
    char stringdata0[276];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Book_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Book_t qt_meta_stringdata_Book = {
    {
QT_MOC_LITERAL(0, 0, 4), // "Book"
QT_MOC_LITERAL(1, 5, 15), // "fileListUpdated"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 9), // "createDir"
QT_MOC_LITERAL(4, 32, 9), // "flushFile"
QT_MOC_LITERAL(5, 42, 6), // "delDir"
QT_MOC_LITERAL(6, 49, 10), // "renameFile"
QT_MOC_LITERAL(7, 60, 8), // "enterDir"
QT_MOC_LITERAL(8, 69, 11), // "QModelIndex"
QT_MOC_LITERAL(9, 81, 5), // "index"
QT_MOC_LITERAL(10, 87, 9), // "returnPre"
QT_MOC_LITERAL(11, 97, 10), // "uploadFile"
QT_MOC_LITERAL(12, 108, 14), // "uploadFileData"
QT_MOC_LITERAL(13, 123, 10), // "delRegFile"
QT_MOC_LITERAL(14, 134, 12), // "downloadFile"
QT_MOC_LITERAL(15, 147, 9), // "shareFile"
QT_MOC_LITERAL(16, 157, 8), // "moveFile"
QT_MOC_LITERAL(17, 166, 13), // "selectDestDir"
QT_MOC_LITERAL(18, 180, 10), // "searchFile"
QT_MOC_LITERAL(19, 191, 23), // "handleSearchFileRespond"
QT_MOC_LITERAL(20, 215, 10), // "const PDU*"
QT_MOC_LITERAL(21, 226, 3), // "pdu"
QT_MOC_LITERAL(22, 230, 23), // "handleSearchResultClick"
QT_MOC_LITERAL(23, 254, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(24, 271, 4) // "item"

    },
    "Book\0fileListUpdated\0\0createDir\0"
    "flushFile\0delDir\0renameFile\0enterDir\0"
    "QModelIndex\0index\0returnPre\0uploadFile\0"
    "uploadFileData\0delRegFile\0downloadFile\0"
    "shareFile\0moveFile\0selectDestDir\0"
    "searchFile\0handleSearchFileRespond\0"
    "const PDU*\0pdu\0handleSearchResultClick\0"
    "QListWidgetItem*\0item"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Book[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,  100,    2, 0x0a /* Public */,
       4,    0,  101,    2, 0x0a /* Public */,
       5,    0,  102,    2, 0x0a /* Public */,
       6,    0,  103,    2, 0x0a /* Public */,
       7,    1,  104,    2, 0x0a /* Public */,
      10,    0,  107,    2, 0x0a /* Public */,
      11,    0,  108,    2, 0x0a /* Public */,
      12,    0,  109,    2, 0x0a /* Public */,
      13,    0,  110,    2, 0x0a /* Public */,
      14,    0,  111,    2, 0x0a /* Public */,
      15,    0,  112,    2, 0x0a /* Public */,
      16,    0,  113,    2, 0x0a /* Public */,
      17,    0,  114,    2, 0x0a /* Public */,
      18,    0,  115,    2, 0x0a /* Public */,
      19,    1,  116,    2, 0x0a /* Public */,
      22,    1,  119,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void, 0x80000000 | 23,   24,

       0        // eod
};

void Book::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Book *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fileListUpdated(); break;
        case 1: _t->createDir(); break;
        case 2: _t->flushFile(); break;
        case 3: _t->delDir(); break;
        case 4: _t->renameFile(); break;
        case 5: _t->enterDir((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 6: _t->returnPre(); break;
        case 7: _t->uploadFile(); break;
        case 8: _t->uploadFileData(); break;
        case 9: _t->delRegFile(); break;
        case 10: _t->downloadFile(); break;
        case 11: _t->shareFile(); break;
        case 12: _t->moveFile(); break;
        case 13: _t->selectDestDir(); break;
        case 14: _t->searchFile(); break;
        case 15: _t->handleSearchFileRespond((*reinterpret_cast< const PDU*(*)>(_a[1]))); break;
        case 16: _t->handleSearchResultClick((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Book::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Book::fileListUpdated)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Book::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_Book.data,
    qt_meta_data_Book,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Book::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Book::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Book.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Book::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void Book::fileListUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
