/****************************************************************************
** Meta object code from reading C++ file 'dolphindirlister.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../../../library/Ember satellite projects/wreathe-file-manager/src/views/dolphindirlister.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dolphindirlister.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DolphinDirLister_t {
    QByteArrayData data[7];
    char stringdata0[59];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DolphinDirLister_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DolphinDirLister_t qt_meta_stringdata_DolphinDirLister = {
    {
QT_MOC_LITERAL(0, 0, 16), // "DolphinDirLister"
QT_MOC_LITERAL(1, 17, 12), // "errorMessage"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 3), // "msg"
QT_MOC_LITERAL(4, 35, 14), // "urlIsFileError"
QT_MOC_LITERAL(5, 50, 4), // "KUrl"
QT_MOC_LITERAL(6, 55, 3) // "url"

    },
    "DolphinDirLister\0errorMessage\0\0msg\0"
    "urlIsFileError\0KUrl\0url"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DolphinDirLister[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    1,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5,    6,

       0        // eod
};

void DolphinDirLister::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DolphinDirLister *_t = static_cast<DolphinDirLister *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->errorMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->urlIsFileError((*reinterpret_cast< const KUrl(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DolphinDirLister::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DolphinDirLister::errorMessage)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DolphinDirLister::*_t)(const KUrl & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DolphinDirLister::urlIsFileError)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject DolphinDirLister::staticMetaObject = {
    { &KDirLister::staticMetaObject, qt_meta_stringdata_DolphinDirLister.data,
      qt_meta_data_DolphinDirLister,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DolphinDirLister::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DolphinDirLister::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DolphinDirLister.stringdata0))
        return static_cast<void*>(const_cast< DolphinDirLister*>(this));
    return KDirLister::qt_metacast(_clname);
}

int DolphinDirLister::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KDirLister::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void DolphinDirLister::errorMessage(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DolphinDirLister::urlIsFileError(const KUrl & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
