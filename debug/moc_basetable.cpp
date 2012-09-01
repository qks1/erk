/****************************************************************************
** Meta object code from reading C++ file 'basetable.h'
**
** Created: Thu 30. Aug 22:17:53 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../basetable.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'basetable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MyHeaderView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_MyHeaderView[] = {
    "MyHeaderView\0\0mouse_release()\0"
};

void MyHeaderView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MyHeaderView *_t = static_cast<MyHeaderView *>(_o);
        switch (_id) {
        case 0: _t->mouse_release(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MyHeaderView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MyHeaderView::staticMetaObject = {
    { &QHeaderView::staticMetaObject, qt_meta_stringdata_MyHeaderView,
      qt_meta_data_MyHeaderView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MyHeaderView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MyHeaderView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MyHeaderView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyHeaderView))
        return static_cast<void*>(const_cast< MyHeaderView*>(this));
    return QHeaderView::qt_metacast(_clname);
}

int MyHeaderView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QHeaderView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void MyHeaderView::mouse_release()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_BaseTable[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      32,   10,   10,   10, 0x05,
      49,   10,   10,   10, 0x05,
      69,   67,   10,   10, 0x05,
      94,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     110,   10,   10,   10, 0x09,
     127,   10,   10,   10, 0x09,
     146,   10,   10,   10, 0x09,
     162,   10,   10,   10, 0x09,
     182,  179,   10,   10, 0x09,
     216,  179,   10,   10, 0x09,
     242,   10,   10,   10, 0x08,
     255,   10,   10,   10, 0x08,
     268,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BaseTable[] = {
    "BaseTable\0\0limits_changed(pair)\0"
    "limits_removed()\0limits_restored()\0,\0"
    "section_resized(int,int)\0section_moved()\0"
    "change_page(int)\0change_onpage(int)\0"
    "remove_limits()\0restore_limits()\0,,\0"
    "column_width_changed(int,int,int)\0"
    "column_moved(int,int,int)\0save_state()\0"
    "save_order()\0section_resized_slot()\0"
};

void BaseTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BaseTable *_t = static_cast<BaseTable *>(_o);
        switch (_id) {
        case 0: _t->limits_changed((*reinterpret_cast< pair(*)>(_a[1]))); break;
        case 1: _t->limits_removed(); break;
        case 2: _t->limits_restored(); break;
        case 3: _t->section_resized((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->section_moved(); break;
        case 5: _t->change_page((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->change_onpage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->remove_limits(); break;
        case 8: _t->restore_limits(); break;
        case 9: _t->column_width_changed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 10: _t->column_moved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 11: _t->save_state(); break;
        case 12: _t->save_order(); break;
        case 13: _t->section_resized_slot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BaseTable::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BaseTable::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_BaseTable,
      qt_meta_data_BaseTable, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BaseTable::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BaseTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BaseTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BaseTable))
        return static_cast<void*>(const_cast< BaseTable*>(this));
    return QWidget::qt_metacast(_clname);
}

int BaseTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void BaseTable::limits_changed(pair _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BaseTable::limits_removed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void BaseTable::limits_restored()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void BaseTable::section_resized(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BaseTable::section_moved()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
