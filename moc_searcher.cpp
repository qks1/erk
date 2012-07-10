/****************************************************************************
** Meta object code from reading C++ file 'searcher.h'
**
** Created: Tue Jul 10 17:51:45 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "searcher.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'searcher.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Searcher[] = {

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
      12,   10,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,    9,    9,    9, 0x08,
      62,    9,    9,    9, 0x08,
      86,    9,    9,    9, 0x08,
     101,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Searcher[] = {
    "Searcher\0\0,\0fill(QSqlQuery,QStringList)\0"
    "set_group_filter(int)\0set_limits_filter(pair)\0"
    "set_nolimits()\0set_limits()\0"
};

void Searcher::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Searcher *_t = static_cast<Searcher *>(_o);
        switch (_id) {
        case 0: _t->fill((*reinterpret_cast< QSqlQuery(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 1: _t->set_group_filter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->set_limits_filter((*reinterpret_cast< pair(*)>(_a[1]))); break;
        case 3: _t->set_nolimits(); break;
        case 4: _t->set_limits(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Searcher::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Searcher::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Searcher,
      qt_meta_data_Searcher, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Searcher::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Searcher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Searcher::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Searcher))
        return static_cast<void*>(const_cast< Searcher*>(this));
    return QWidget::qt_metacast(_clname);
}

int Searcher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void Searcher::fill(QSqlQuery _t1, QStringList _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
