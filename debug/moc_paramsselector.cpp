/****************************************************************************
** Meta object code from reading C++ file 'paramsselector.h'
**
** Created: Mon 10. Sep 18:31:50 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../searcher/paramsselector.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'paramsselector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ParamsSelector[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   16,   15,   15, 0x05,
      45,   15,   15,   15, 0x05,
      65,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      86,   15,   15,   15, 0x08,
     107,   15,   15,   15, 0x08,
     122,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ParamsSelector[] = {
    "ParamsSelector\0\0,\0item_selected(QString,int)\0"
    "button_clicked(int)\0fill_box_signal(int)\0"
    "select_item(QString)\0click_button()\0"
    "fill_box_slot()\0"
};

void ParamsSelector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ParamsSelector *_t = static_cast<ParamsSelector *>(_o);
        switch (_id) {
        case 0: _t->item_selected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->button_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->fill_box_signal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->select_item((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->click_button(); break;
        case 5: _t->fill_box_slot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ParamsSelector::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ParamsSelector::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ParamsSelector,
      qt_meta_data_ParamsSelector, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ParamsSelector::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ParamsSelector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ParamsSelector::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ParamsSelector))
        return static_cast<void*>(const_cast< ParamsSelector*>(this));
    return QWidget::qt_metacast(_clname);
}

int ParamsSelector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void ParamsSelector::item_selected(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ParamsSelector::button_clicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ParamsSelector::fill_box_signal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
