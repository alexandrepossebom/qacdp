/****************************************************************************
** Meta object code from reading C++ file 'acdp.h'
**
** Created: Fri Sep 4 14:36:06 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "acdp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'acdp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ACDP[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      11,    5,    6,    5, 0x08,
      27,    5,    5,    5, 0x08,
      67,    5,    5,    5, 0x08,
      86,    5,    5,    5, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ACDP[] = {
    "ACDP\0\0bool\0loginDone(bool)\0"
    "readResponseHeader(QHttpResponseHeader)\0"
    "projetosDone(bool)\0sendDone(bool)\0"
};

const QMetaObject ACDP::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ACDP,
      qt_meta_data_ACDP, 0 }
};

const QMetaObject *ACDP::metaObject() const
{
    return &staticMetaObject;
}

void *ACDP::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ACDP))
        return static_cast<void*>(const_cast< ACDP*>(this));
    return QObject::qt_metacast(_clname);
}

int ACDP::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { bool _r = loginDone((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: readResponseHeader((*reinterpret_cast< const QHttpResponseHeader(*)>(_a[1]))); break;
        case 2: projetosDone((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: sendDone((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
