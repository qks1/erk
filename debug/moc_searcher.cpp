/****************************************************************************
** Meta object code from reading C++ file 'searcher.h'
**
** Created: Mon 10. Sep 18:32:37 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../searcher/searcher.h"
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
      48,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   10,    9,    9, 0x05,
      40,    9,    9,    9, 0x05,
      63,   60,    9,    9, 0x05,
      92,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
     111,    9,    9,    9, 0x08,
     126,    9,    9,    9, 0x08,
     148,    9,    9,    9, 0x08,
     169,    9,    9,    9, 0x08,
     193,    9,    9,    9, 0x08,
     208,    9,    9,    9, 0x08,
     221,   60,    9,    9, 0x08,
     254,    9,    9,    9, 0x08,
     278,    9,    9,    9, 0x08,
     305,    9,    9,    9, 0x08,
     331,   10,    9,    9, 0x08,
     368,    9,    9,    9, 0x08,
     385,   10,    9,    9, 0x08,
     415,    9,    9,    9, 0x08,
     439,    9,    9,    9, 0x08,
     461,    9,    9,    9, 0x08,
     484,    9,    9,    9, 0x08,
     509,    9,    9,    9, 0x08,
     530,    9,    9,    9, 0x08,
     545,    9,    9,    9, 0x08,
     569,    9,    9,    9, 0x08,
     598,    9,    9,    9, 0x08,
     618,    9,    9,    9, 0x08,
     636,   10,    9,    9, 0x08,
     678,    9,    9,    9, 0x08,
     707,    9,    9,    9, 0x08,
     733,    9,    9,    9, 0x08,
     760,    9,    9,    9, 0x08,
     785,    9,    9,    9, 0x08,
     804,   10,    9,    9, 0x08,
     838,   10,    9,    9, 0x08,
     873,   10,    9,    9, 0x08,
     906,    9,    9,    9, 0x08,
     925,    9,    9,    9, 0x08,
     951,    9,    9,    9, 0x08,
     969,    9,    9,    9, 0x08,
     999,    9,    9,    9, 0x08,
    1027,    9,    9,    9, 0x08,
    1057,    9,    9,    9, 0x08,
    1075,    9,    9,    9, 0x08,
    1093,   10,    9,    9, 0x08,
    1124,   10,    9,    9, 0x08,
    1154,    9,    9,    9, 0x08,
    1176,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Searcher[] = {
    "Searcher\0\0,\0fill(QSqlQuery,QStringList)\0"
    "clear_text_signal()\0,,\0"
    "section_resized(int,int,int)\0"
    "section_moved(int)\0refresh_grey()\0"
    "set_group_filter(int)\0reset_group_filter()\0"
    "set_limits_filter(pair)\0set_nolimits()\0"
    "set_limits()\0set_text_filter(int,int,QString)\0"
    "reset_text_filters(int)\0"
    "reset_white_text_filters()\0"
    "reset_grey_text_filters()\0"
    "change_sort_order(int,Qt::SortOrder)\0"
    "button_sort(int)\0set_param_filter(QString,int)\0"
    "reset_param_filter(int)\0reset_params_filter()\0"
    "switch_prices_filter()\0switch_quantity_filter()\0"
    "set_last_filter(int)\0open_grey(int)\0"
    "close_grey(QModelIndex)\0"
    "set_grey_limits_filter(pair)\0"
    "set_grey_nolimits()\0set_grey_limits()\0"
    "change_grey_sort_order(int,Qt::SortOrder)\0"
    "change_grey_storage(QString)\0"
    "change_grey_rack(QString)\0"
    "change_grey_board(QString)\0"
    "change_grey_box(QString)\0reset_grey_boxes()\0"
    "change_grey_one_year(QString,int)\0"
    "change_grey_from_year(QString,int)\0"
    "change_grey_to_year(QString,int)\0"
    "reset_grey_years()\0change_grey_insp(QString)\0"
    "reset_grey_insp()\0change_grey_add_info(QString)\0"
    "change_grey_defect(QString)\0"
    "change_grey_category(QString)\0"
    "reset_add_boxes()\0grey_reset_slot()\0"
    "white_section_resized(int,int)\0"
    "grey_section_resized(int,int)\0"
    "white_section_moved()\0grey_section_moved()\0"
};

void Searcher::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Searcher *_t = static_cast<Searcher *>(_o);
        switch (_id) {
        case 0: _t->fill((*reinterpret_cast< QSqlQuery(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 1: _t->clear_text_signal(); break;
        case 2: _t->section_resized((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->section_moved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->refresh_grey(); break;
        case 5: _t->set_group_filter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->reset_group_filter(); break;
        case 7: _t->set_limits_filter((*reinterpret_cast< pair(*)>(_a[1]))); break;
        case 8: _t->set_nolimits(); break;
        case 9: _t->set_limits(); break;
        case 10: _t->set_text_filter((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 11: _t->reset_text_filters((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->reset_white_text_filters(); break;
        case 13: _t->reset_grey_text_filters(); break;
        case 14: _t->change_sort_order((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Qt::SortOrder(*)>(_a[2]))); break;
        case 15: _t->button_sort((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->set_param_filter((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 17: _t->reset_param_filter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->reset_params_filter(); break;
        case 19: _t->switch_prices_filter(); break;
        case 20: _t->switch_quantity_filter(); break;
        case 21: _t->set_last_filter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->open_grey((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->close_grey((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 24: _t->set_grey_limits_filter((*reinterpret_cast< pair(*)>(_a[1]))); break;
        case 25: _t->set_grey_nolimits(); break;
        case 26: _t->set_grey_limits(); break;
        case 27: _t->change_grey_sort_order((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Qt::SortOrder(*)>(_a[2]))); break;
        case 28: _t->change_grey_storage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 29: _t->change_grey_rack((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 30: _t->change_grey_board((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 31: _t->change_grey_box((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 32: _t->reset_grey_boxes(); break;
        case 33: _t->change_grey_one_year((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 34: _t->change_grey_from_year((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 35: _t->change_grey_to_year((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 36: _t->reset_grey_years(); break;
        case 37: _t->change_grey_insp((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 38: _t->reset_grey_insp(); break;
        case 39: _t->change_grey_add_info((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 40: _t->change_grey_defect((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 41: _t->change_grey_category((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 42: _t->reset_add_boxes(); break;
        case 43: _t->grey_reset_slot(); break;
        case 44: _t->white_section_resized((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 45: _t->grey_section_resized((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 46: _t->white_section_moved(); break;
        case 47: _t->grey_section_moved(); break;
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
        if (_id < 48)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 48;
    }
    return _id;
}

// SIGNAL 0
void Searcher::fill(QSqlQuery _t1, QStringList _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Searcher::clear_text_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Searcher::section_resized(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Searcher::section_moved(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
