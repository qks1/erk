/****************************************************************************
** Meta object code from reading C++ file 'table.h'
**
** Created: Mon Jul 9 12:30:42 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "table.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'table.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Table[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x05,
      28,    6,    6,    6, 0x05,
      45,    6,    6,    6, 0x05,

 // slots: signature, parameters, type, tag, flags
      63,    6,    6,    6, 0x08,
      80,    6,    6,    6, 0x08,
      99,    6,    6,    6, 0x08,
     115,    6,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Table[] = {
    "Table\0\0limits_changed(pair)\0"
    "limits_removed()\0limits_restored()\0"
    "change_page(int)\0change_onpage(int)\0"
    "remove_limits()\0restore_limits()\0"
};

void Table::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Table *_t = static_cast<Table *>(_o);
        switch (_id) {
        case 0: _t->limits_changed((*reinterpret_cast< pair(*)>(_a[1]))); break;
        case 1: _t->limits_removed(); break;
        case 2: _t->limits_restored(); break;
        case 3: _t->change_page((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->change_onpage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->remove_limits(); break;
        case 6: _t->restore_limits(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Table::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Table::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Table,
      qt_meta_data_Table, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Table::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Table::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Table::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Table))
        return static_cast<void*>(const_cast< Table*>(this));
    return QWidget::qt_metacast(_clname);
}

int Table::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Table::limits_changed(pair _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Table::limits_removed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Table::limits_restored()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
