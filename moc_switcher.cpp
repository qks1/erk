/****************************************************************************
** Meta object code from reading C++ file 'switcher.h'
**
** Created: Thu Jul 5 17:10:31 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "switcher.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'switcher.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Switcher[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      28,    9,    9,    9, 0x08,
      44,    9,    9,    9, 0x08,
      59,    9,    9,    9, 0x08,
      74,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Switcher[] = {
    "Switcher\0\0page_changed(int)\0first_clicked()\0"
    "prev_clicked()\0next_clicked()\0"
    "last_clicked()\0"
};

void Switcher::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Switcher *_t = static_cast<Switcher *>(_o);
        switch (_id) {
        case 0: _t->page_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->first_clicked(); break;
        case 2: _t->prev_clicked(); break;
        case 3: _t->next_clicked(); break;
        case 4: _t->last_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Switcher::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Switcher::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Switcher,
      qt_meta_data_Switcher, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Switcher::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Switcher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Switcher::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Switcher))
        return static_cast<void*>(const_cast< Switcher*>(this));
    return QWidget::qt_metacast(_clname);
}

int Switcher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Switcher::page_changed(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
