/****************************************************************************
** Meta object code from reading C++ file 'table.h'
**
** Created: Mon Aug 13 13:21:38 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "searcher/table.h"
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
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    7,    6,    6, 0x05,
      47,    6,    6,    6, 0x05,
      64,    6,    6,    6, 0x05,
      83,    6,    6,    6, 0x05,

 // slots: signature, parameters, type, tag, flags
     100,    6,    6,    6, 0x08,
     127,    6,    6,    6, 0x08,
     156,  153,    6,    6, 0x08,
     190,  153,    6,    6, 0x08,
     216,    7,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Table[] = {
    "Table\0\0,\0sort_order_changed(int,Qt::SortOrder)\0"
    "prices_clicked()\0quantity_clicked()\0"
    "right_click(int)\0header_right_click(QPoint)\0"
    "table_right_click(QPoint)\0,,\0"
    "column_width_changed(int,int,int)\0"
    "column_moved(int,int,int)\0"
    "change_order(int,Qt::SortOrder)\0"
};

void Table::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Table *_t = static_cast<Table *>(_o);
        switch (_id) {
        case 0: _t->sort_order_changed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Qt::SortOrder(*)>(_a[2]))); break;
        case 1: _t->prices_clicked(); break;
        case 2: _t->quantity_clicked(); break;
        case 3: _t->right_click((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->header_right_click((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 5: _t->table_right_click((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 6: _t->column_width_changed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: _t->column_moved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 8: _t->change_order((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Qt::SortOrder(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Table::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Table::staticMetaObject = {
    { &BaseTable::staticMetaObject, qt_meta_stringdata_Table,
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
    return BaseTable::qt_metacast(_clname);
}

int Table::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseTable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Table::sort_order_changed(int _t1, Qt::SortOrder _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Table::prices_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Table::quantity_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Table::right_click(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
