/****************************************************************************
** Meta object code from reading C++ file 'greysearcher.h'
**
** Created: Mon 10. Sep 18:30:13 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../searcher/greysearcher.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'greysearcher.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GreySearcher[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      48,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      28,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      38,   13,   13,   13, 0x05,
      63,   13,   13,   13, 0x05,
      84,   13,   13,   13, 0x05,
     101,   13,   13,   13, 0x05,
     121,  119,   13,   13, 0x05,
     162,  159,   13,   13, 0x05,
     199,   13,   13,   13, 0x05,
     214,   13,   13,   13, 0x05,
     238,   13,   13,   13, 0x05,
     259,   13,   13,   13, 0x05,
     281,   13,   13,   13, 0x05,
     301,   13,   13,   13, 0x05,
     326,  322,   13,   13, 0x05,
     373,  119,   13,   13, 0x05,
     409,  119,   13,   13, 0x05,
     446,  119,   13,   13, 0x05,
     481,   13,   13,   13, 0x05,
     491,   13,   13,   13, 0x05,
     505,   13,   13,   13, 0x05,
     533,   13,   13,   13, 0x05,
     553,   13,   13,   13, 0x05,
     585,   13,   13,   13, 0x05,
     615,   13,   13,   13, 0x05,
     647,   13,   13,   13, 0x05,
     672,   13,   13,   13, 0x05,
     693,  119,   13,   13, 0x05,
     718,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     734,  119,   13,   13, 0x08,
     765,   13,   13,   13, 0x08,
     783,   13,   13,   13, 0x08,
     801,   13,   13,   13, 0x08,
     815,   13,   13,   13, 0x08,
     841,   13,   13,   13, 0x08,
     868,   13,   13,   13, 0x08,
     893,   13,   13,   13, 0x08,
     913,   13,   13,   13, 0x08,
     934,   13,   13,   13, 0x08,
     953,   13,   13,   13, 0x08,
     973,   13,   13,   13, 0x08,
     990,   13,   13,   13, 0x08,
    1008,   13,   13,   13, 0x08,
    1025,   13,   13,   13, 0x08,
    1044,   13,   13,   13, 0x08,
    1062,   13,   13,   13, 0x08,
    1082,   13,   13,   13, 0x08,
    1100,   13,   13,   13, 0x08,
    1120,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GreySearcher[] = {
    "GreySearcher\0\0close_grey(QModelIndex)\0"
    "last_filter_changed(int)\0limits_changed(pair)\0"
    "limits_removed()\0limits_restored()\0,\0"
    "sort_order_changed(int,Qt::SortOrder)\0"
    ",,\0text_changed_signal(int,int,QString)\0"
    "reset_signal()\0storage_signal(QString)\0"
    "rack_signal(QString)\0board_signal(QString)\0"
    "box_signal(QString)\0reset_boxes_signal()\0"
    ",,,\0just_change_place(QString,QString,int,QString)\0"
    "change_one_year_signal(QString,int)\0"
    "change_from_year_signal(QString,int)\0"
    "change_to_year_signal(QString,int)\0"
    "refresh()\0reset_years()\0"
    "change_insp_signal(QString)\0"
    "reset_insp_signal()\0change_add_info_signal(QString)\0"
    "change_defect_signal(QString)\0"
    "change_category_signal(QString)\0"
    "reset_add_boxes_signal()\0total_reset_signal()\0"
    "section_resized(int,int)\0section_moved()\0"
    "text_changed_slot(int,QString)\0"
    "show_second_box()\0hide_second_box()\0"
    "change_mode()\0one_year_changed(QString)\0"
    "from_year_changed(QString)\0"
    "to_year_changed(QString)\0fill_one_year_box()\0"
    "fill_from_year_box()\0fill_to_year_box()\0"
    "fill_storages_box()\0fill_racks_box()\0"
    "fill_boards_box()\0fill_boxes_box()\0"
    "reset_boxes_slot()\0change_add_mode()\0"
    "fill_add_info_box()\0fill_defect_box()\0"
    "fill_category_box()\0total_reset_slot()\0"
};

void GreySearcher::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GreySearcher *_t = static_cast<GreySearcher *>(_o);
        switch (_id) {
        case 0: _t->close_grey((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 1: _t->last_filter_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->limits_changed((*reinterpret_cast< pair(*)>(_a[1]))); break;
        case 3: _t->limits_removed(); break;
        case 4: _t->limits_restored(); break;
        case 5: _t->sort_order_changed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Qt::SortOrder(*)>(_a[2]))); break;
        case 6: _t->text_changed_signal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: _t->reset_signal(); break;
        case 8: _t->storage_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->rack_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->board_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->box_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->reset_boxes_signal(); break;
        case 13: _t->just_change_place((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 14: _t->change_one_year_signal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->change_from_year_signal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 16: _t->change_to_year_signal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 17: _t->refresh(); break;
        case 18: _t->reset_years(); break;
        case 19: _t->change_insp_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: _t->reset_insp_signal(); break;
        case 21: _t->change_add_info_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 22: _t->change_defect_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->change_category_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 24: _t->reset_add_boxes_signal(); break;
        case 25: _t->total_reset_signal(); break;
        case 26: _t->section_resized((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 27: _t->section_moved(); break;
        case 28: _t->text_changed_slot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 29: _t->show_second_box(); break;
        case 30: _t->hide_second_box(); break;
        case 31: _t->change_mode(); break;
        case 32: _t->one_year_changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 33: _t->from_year_changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 34: _t->to_year_changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 35: _t->fill_one_year_box(); break;
        case 36: _t->fill_from_year_box(); break;
        case 37: _t->fill_to_year_box(); break;
        case 38: _t->fill_storages_box(); break;
        case 39: _t->fill_racks_box(); break;
        case 40: _t->fill_boards_box(); break;
        case 41: _t->fill_boxes_box(); break;
        case 42: _t->reset_boxes_slot(); break;
        case 43: _t->change_add_mode(); break;
        case 44: _t->fill_add_info_box(); break;
        case 45: _t->fill_defect_box(); break;
        case 46: _t->fill_category_box(); break;
        case 47: _t->total_reset_slot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GreySearcher::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GreySearcher::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GreySearcher,
      qt_meta_data_GreySearcher, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GreySearcher::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GreySearcher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GreySearcher::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GreySearcher))
        return static_cast<void*>(const_cast< GreySearcher*>(this));
    return QWidget::qt_metacast(_clname);
}

int GreySearcher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void GreySearcher::close_grey(QModelIndex _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GreySearcher::last_filter_changed(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GreySearcher::limits_changed(pair _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GreySearcher::limits_removed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void GreySearcher::limits_restored()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void GreySearcher::sort_order_changed(int _t1, Qt::SortOrder _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void GreySearcher::text_changed_signal(int _t1, int _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void GreySearcher::reset_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void GreySearcher::storage_signal(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void GreySearcher::rack_signal(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void GreySearcher::board_signal(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void GreySearcher::box_signal(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void GreySearcher::reset_boxes_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void GreySearcher::just_change_place(QString _t1, QString _t2, int _t3, QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void GreySearcher::change_one_year_signal(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void GreySearcher::change_from_year_signal(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void GreySearcher::change_to_year_signal(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void GreySearcher::refresh()
{
    QMetaObject::activate(this, &staticMetaObject, 17, 0);
}

// SIGNAL 18
void GreySearcher::reset_years()
{
    QMetaObject::activate(this, &staticMetaObject, 18, 0);
}

// SIGNAL 19
void GreySearcher::change_insp_signal(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void GreySearcher::reset_insp_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 20, 0);
}

// SIGNAL 21
void GreySearcher::change_add_info_signal(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void GreySearcher::change_defect_signal(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void GreySearcher::change_category_signal(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void GreySearcher::reset_add_boxes_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 24, 0);
}

// SIGNAL 25
void GreySearcher::total_reset_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 25, 0);
}

// SIGNAL 26
void GreySearcher::section_resized(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void GreySearcher::section_moved()
{
    QMetaObject::activate(this, &staticMetaObject, 27, 0);
}
static const uint qt_meta_data_TableModelForYears[] = {

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

static const char qt_meta_stringdata_TableModelForYears[] = {
    "TableModelForYears\0"
};

void TableModelForYears::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData TableModelForYears::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TableModelForYears::staticMetaObject = {
    { &QSqlQueryModel::staticMetaObject, qt_meta_stringdata_TableModelForYears,
      qt_meta_data_TableModelForYears, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TableModelForYears::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TableModelForYears::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TableModelForYears::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TableModelForYears))
        return static_cast<void*>(const_cast< TableModelForYears*>(this));
    return QSqlQueryModel::qt_metacast(_clname);
}

int TableModelForYears::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSqlQueryModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
