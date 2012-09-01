/****************************************************************************
** Meta object code from reading C++ file 'greytable.h'
**
** Created: Tue Aug 28 15:41:57 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "searcher/greytable.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'greytable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GreyTable[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   11,   10,   10, 0x05,
      51,   10,   10,   10, 0x05,
      68,   10,   10,   10, 0x05,
      87,   10,   10,   10, 0x05,
     104,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     130,   10,   10,   10, 0x08,
     156,   11,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GreyTable[] = {
    "GreyTable\0\0,\0sort_order_changed(int,Qt::SortOrder)\0"
    "prices_clicked()\0quantity_clicked()\0"
    "right_click(int)\0double_click(QModelIndex)\0"
    "table_right_click(QPoint)\0"
    "change_order(int,Qt::SortOrder)\0"
};

void GreyTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GreyTable *_t = static_cast<GreyTable *>(_o);
        switch (_id) {
        case 0: _t->sort_order_changed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Qt::SortOrder(*)>(_a[2]))); break;
        case 1: _t->prices_clicked(); break;
        case 2: _t->quantity_clicked(); break;
        case 3: _t->right_click((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->double_click((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 5: _t->table_right_click((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 6: _t->change_order((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Qt::SortOrder(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GreyTable::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GreyTable::staticMetaObject = {
    { &BaseTable::staticMetaObject, qt_meta_stringdata_GreyTable,
      qt_meta_data_GreyTable, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GreyTable::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GreyTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GreyTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GreyTable))
        return static_cast<void*>(const_cast< GreyTable*>(this));
    return BaseTable::qt_metacast(_clname);
}

int GreyTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseTable::qt_metacall(_c, _id, _a);
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
void GreyTable::sort_order_changed(int _t1, Qt::SortOrder _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GreyTable::prices_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void GreyTable::quantity_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void GreyTable::right_click(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void GreyTable::double_click(QModelIndex _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
