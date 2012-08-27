/****************************************************************************
** Meta object code from reading C++ file 'catalog.h'
**
** Created: Mon Aug 27 10:59:37 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "searcher/catalog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'catalog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Catalog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      30,   28,    8,    8, 0x08,
      65,    8,    8,    8, 0x08,
      79,   28,    8,    8, 0x08,
     115,    8,    8,    8, 0x08,
     130,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Catalog[] = {
    "Catalog\0\0group_changed(int)\0,\0"
    "change_group(QTreeWidgetItem*,int)\0"
    "clear_group()\0change_header(QTreeWidgetItem*,int)\0"
    "clear_header()\0move_button()\0"
};

void Catalog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Catalog *_t = static_cast<Catalog *>(_o);
        switch (_id) {
        case 0: _t->group_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->change_group((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->clear_group(); break;
        case 3: _t->change_header((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->clear_header(); break;
        case 5: _t->move_button(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Catalog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Catalog::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Catalog,
      qt_meta_data_Catalog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Catalog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Catalog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Catalog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Catalog))
        return static_cast<void*>(const_cast< Catalog*>(this));
    return QWidget::qt_metacast(_clname);
}

int Catalog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Catalog::group_changed(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
