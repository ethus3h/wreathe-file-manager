/****************************************************************************
** Meta object code from reading C++ file 'kversioncontrolplugin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../../../library/Ember satellite projects/wreathe-file-manager/src/views/versioncontrol/kversioncontrolplugin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kversioncontrolplugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_KVersionControlPlugin_t {
    QByteArrayData data[7];
    char stringdata0[98];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_KVersionControlPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_KVersionControlPlugin_t qt_meta_stringdata_KVersionControlPlugin = {
    {
QT_MOC_LITERAL(0, 0, 21), // "KVersionControlPlugin"
QT_MOC_LITERAL(1, 22, 19), // "itemVersionsChanged"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 11), // "infoMessage"
QT_MOC_LITERAL(4, 55, 3), // "msg"
QT_MOC_LITERAL(5, 59, 12), // "errorMessage"
QT_MOC_LITERAL(6, 72, 25) // "operationCompletedMessage"

    },
    "KVersionControlPlugin\0itemVersionsChanged\0"
    "\0infoMessage\0msg\0errorMessage\0"
    "operationCompletedMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_KVersionControlPlugin[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    1,   35,    2, 0x06 /* Public */,
       5,    1,   38,    2, 0x06 /* Public */,
       6,    1,   41,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,

       0        // eod
};

void KVersionControlPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        KVersionControlPlugin *_t = static_cast<KVersionControlPlugin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->itemVersionsChanged(); break;
        case 1: _t->infoMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->errorMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->operationCompletedMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (KVersionControlPlugin::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&KVersionControlPlugin::itemVersionsChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (KVersionControlPlugin::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&KVersionControlPlugin::infoMessage)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (KVersionControlPlugin::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&KVersionControlPlugin::errorMessage)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (KVersionControlPlugin::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&KVersionControlPlugin::operationCompletedMessage)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject KVersionControlPlugin::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_KVersionControlPlugin.data,
      qt_meta_data_KVersionControlPlugin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *KVersionControlPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *KVersionControlPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_KVersionControlPlugin.stringdata0))
        return static_cast<void*>(const_cast< KVersionControlPlugin*>(this));
    return QObject::qt_metacast(_clname);
}

int KVersionControlPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void KVersionControlPlugin::itemVersionsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void KVersionControlPlugin::infoMessage(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void KVersionControlPlugin::errorMessage(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void KVersionControlPlugin::operationCompletedMessage(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE