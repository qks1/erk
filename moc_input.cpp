/****************************************************************************
** Meta object code from reading C++ file 'input.h'
**
** Created: Tue Aug 28 15:41:59 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "searcher/input.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'input.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Input[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    7,    6,    6, 0x05,
      42,    6,    6,    6, 0x05,

 // slots: signature, parameters, type, tag, flags
      57,    6,    6,    6, 0x0a,
      84,    6,    6,    6, 0x0a,
     110,    6,    6,    6, 0x0a,
     128,    6,    6,    6, 0x0a,
     141,    6,    6,    6, 0x0a,
     154,    6,    6,    6, 0x08,
     166,    6,    6,    6, 0x08,
     178,    6,    6,    6, 0x08,
     200,    6,    6,    6, 0x08,
     216,    6,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Input[] = {
    "Input\0\0,\0text_changed_signal(int,QString)\0"
    "reset_signal()\0text_changed_slot(QString)\0"
    "id_mode_changed_slot(int)\0set_text(QString)\0"
    "reset_text()\0send_reset()\0hide_eqge()\0"
    "show_eqge()\0set_cursor_to_begin()\0"
    "set_validator()\0remove_validator()\0"
};

void Input::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Input *_t = static_cast<Input *>(_o);
        switch (_id) {
        case 0: _t->text_changed_signal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->reset_signal(); break;
        case 2: _t->text_changed_slot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->id_mode_changed_slot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->set_text((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->reset_text(); break;
        case 6: _t->send_reset(); break;
        case 7: _t->hide_eqge(); break;
        case 8: _t->show_eqge(); break;
        case 9: _t->set_cursor_to_begin(); break;
        case 10: _t->set_validator(); break;
        case 11: _t->remove_validator(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Input::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Input::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Input,
      qt_meta_data_Input, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Input::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Input::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Input::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Input))
        return static_cast<void*>(const_cast< Input*>(this));
    return QWidget::qt_metacast(_clname);
}

int Input::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Input::text_changed_signal(int _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Input::reset_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
