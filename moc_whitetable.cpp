/****************************************************************************
** Meta object code from reading C++ file 'whitetable.h'
**
** Created: Tue Aug 28 15:42:07 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "searcher/whitetable.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'whitetable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WhiteTableModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_WhiteTableModel[] = {
    "WhiteTableModel\0"
};

void WhiteTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData WhiteTableModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WhiteTableModel::staticMetaObject = {
    { &MyTableModel::staticMetaObject, qt_meta_stringdata_WhiteTableModel,
      qt_meta_data_WhiteTableModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WhiteTableModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WhiteTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WhiteTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WhiteTableModel))
        return static_cast<void*>(const_cast< WhiteTableModel*>(this));
    return MyTableModel::qt_metacast(_clname);
}

int WhiteTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MyTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_WhiteTable[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   12,   11,   11, 0x05,
      52,   11,   11,   11, 0x05,
      69,   11,   11,   11, 0x05,
      88,   11,   11,   11, 0x05,
     105,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     131,   11,   11,   11, 0x08,
     158,   11,   11,   11, 0x08,
     184,   12,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_WhiteTable[] = {
    "WhiteTable\0\0,\0sort_order_changed(int,Qt::SortOrder)\0"
    "prices_clicked()\0quantity_clicked()\0"
    "right_click(int)\0double_click(QModelIndex)\0"
    "header_right_click(QPoint)\0"
    "table_right_click(QPoint)\0"
    "change_order(int,Qt::SortOrder)\0"
};

void WhiteTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WhiteTable *_t = static_cast<WhiteTable *>(_o);
        switch (_id) {
        case 0: _t->sort_order_changed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Qt::SortOrder(*)>(_a[2]))); break;
        case 1: _t->prices_clicked(); break;
        case 2: _t->quantity_clicked(); break;
        case 3: _t->right_click((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->double_click((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 5: _t->header_right_click((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 6: _t->table_right_click((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 7: _t->change_order((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Qt::SortOrder(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WhiteTable::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WhiteTable::staticMetaObject = {
    { &BaseTable::staticMetaObject, qt_meta_stringdata_WhiteTable,
      qt_meta_data_WhiteTable, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WhiteTable::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WhiteTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WhiteTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WhiteTable))
        return static_cast<void*>(const_cast< WhiteTable*>(this));
    return BaseTable::qt_metacast(_clname);
}

int WhiteTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseTable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void WhiteTable::sort_order_changed(int _t1, Qt::SortOrder _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WhiteTable::prices_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void WhiteTable::quantity_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void WhiteTable::right_click(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void WhiteTable::double_click(QModelIndex _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
