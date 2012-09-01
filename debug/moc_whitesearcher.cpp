/****************************************************************************
** Meta object code from reading C++ file 'whitesearcher.h'
**
** Created: Thu 30. Aug 22:17:57 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../searcher/whitesearcher.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'whitesearcher.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WhiteSearcher[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      17,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   15,   14,   14, 0x05,
      45,   14,   14,   14, 0x05,
      70,   14,   14,   14, 0x05,
      89,   14,   14,   14, 0x05,
     110,   14,   14,   14, 0x05,
     127,   14,   14,   14, 0x05,
     148,  145,   14,   14, 0x05,
     185,   14,   14,   14, 0x05,
     200,   15,   14,   14, 0x05,
     238,   15,   14,   14, 0x05,
     265,   14,   14,   14, 0x05,
     285,   14,   14,   14, 0x05,
     308,   14,   14,   14, 0x05,
     325,   14,   14,   14, 0x05,
     344,   14,   14,   14, 0x05,
     361,   15,   14,   14, 0x05,
     386,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     402,   14,   14,   14, 0x08,
     419,   14,   14,   14, 0x08,
     437,   14,   14,   14, 0x08,
     460,   15,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_WhiteSearcher[] = {
    "WhiteSearcher\0\0,\0fill(QSqlQuery,QStringList)\0"
    "last_filter_changed(int)\0group_changed(int)\0"
    "limits_changed(pair)\0limits_removed()\0"
    "limits_restored()\0,,\0"
    "text_changed_signal(int,int,QString)\0"
    "reset_signal()\0sort_order_changed(int,Qt::SortOrder)\0"
    "item_selected(QString,int)\0"
    "button_clicked(int)\0params_reset_clicked()\0"
    "prices_clicked()\0quantity_clicked()\0"
    "create_grey(int)\0section_resized(int,int)\0"
    "section_moved()\0detail_info(int)\0"
    "fill_boxes_slot()\0open_grey(QModelIndex)\0"
    "text_changed_slot(int,QString)\0"
};

void WhiteSearcher::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WhiteSearcher *_t = static_cast<WhiteSearcher *>(_o);
        switch (_id) {
        case 0: _t->fill((*reinterpret_cast< QSqlQuery(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 1: _t->last_filter_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->group_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->limits_changed((*reinterpret_cast< pair(*)>(_a[1]))); break;
        case 4: _t->limits_removed(); break;
        case 5: _t->limits_restored(); break;
        case 6: _t->text_changed_signal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: _t->reset_signal(); break;
        case 8: _t->sort_order_changed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Qt::SortOrder(*)>(_a[2]))); break;
        case 9: _t->item_selected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->button_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->params_reset_clicked(); break;
        case 12: _t->prices_clicked(); break;
        case 13: _t->quantity_clicked(); break;
        case 14: _t->create_grey((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->section_resized((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 16: _t->section_moved(); break;
        case 17: _t->detail_info((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->fill_boxes_slot(); break;
        case 19: _t->open_grey((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 20: _t->text_changed_slot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WhiteSearcher::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WhiteSearcher::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_WhiteSearcher,
      qt_meta_data_WhiteSearcher, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WhiteSearcher::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WhiteSearcher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WhiteSearcher::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WhiteSearcher))
        return static_cast<void*>(const_cast< WhiteSearcher*>(this));
    return QWidget::qt_metacast(_clname);
}

int WhiteSearcher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void WhiteSearcher::fill(QSqlQuery _t1, QStringList _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WhiteSearcher::last_filter_changed(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void WhiteSearcher::group_changed(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void WhiteSearcher::limits_changed(pair _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void WhiteSearcher::limits_removed()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void WhiteSearcher::limits_restored()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void WhiteSearcher::text_changed_signal(int _t1, int _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void WhiteSearcher::reset_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void WhiteSearcher::sort_order_changed(int _t1, Qt::SortOrder _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void WhiteSearcher::item_selected(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void WhiteSearcher::button_clicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void WhiteSearcher::params_reset_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void WhiteSearcher::prices_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void WhiteSearcher::quantity_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void WhiteSearcher::create_grey(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void WhiteSearcher::section_resized(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void WhiteSearcher::section_moved()
{
    QMetaObject::activate(this, &staticMetaObject, 16, 0);
}
QT_END_MOC_NAMESPACE
