#ifndef HELPERS_H
#define HELPERS_H

#include <QtGui>
#include <QtSql>
#include <QMetaType>
#include "columns.cpp"
#include "constants.h"

class DoubleClickLineEdit : public QLineEdit{
    Q_OBJECT

public:
    DoubleClickLineEdit(QWidget* parent = 0);

private:
    void mouseDoubleClickEvent(QMouseEvent *);

signals:
    void double_clicked();

};

struct pair{
    int begin;
    int end;
};

struct SortingOrder{
    QString column;
    Qt::SortOrder order;
};

// id+текстовая строка. Нужно:
// - для передачи списка деталей в диалог добавления
struct DetailSmall{
    int id;
    QString name;
};


// структура для имён столбцов. Содержит:
//   - список размеров (под размером имеется в виду ширина, но слово "ширин" больно неказистое)
//   - список индексов (соответствий логического и визуального индекса).
//     Так как в sql-запросах порядок столбцов неизменный, логические индексы всегда одни и те же,
//     и этот список определяет порядок столбцов на экране.
//   - список "показывать/не показывать" (соответственно, хранит булевы значения).
//     Управление этим списком в программе осуществляется через меню Внешний вид->Столбцы.
struct ColumnsStruct{
    QList<int> sizes;
    QList<int> indices;
    QList<bool> appears;
};

// Структура, передаваемая в конструктор менеджерского/складского резерва.
// Виджет резерва создаётся отдельно для каждого поисковика, и для их синхронной работы
// они используют одну sql-модель, один список заявок и одно выделение для этого списка
struct ReserveStruct{
    QSqlQueryModel *source_model;
    QStandardItemModel *list_model;
    QItemSelectionModel *selection_model;
    ColumnsStruct columns;
};


// объявляем эту структуру метатипом, чтобы можно было использовать её как QVariant и записывать в QSettings
Q_DECLARE_METATYPE(ColumnsStruct)

bool create_connection(QSqlDatabase);
void set_system_font();
void critical_error(QString title, QString text);
void error(QString title, QString text);
QString sorting_order_to_string(QList<SortingOrder>);
int find_index(QAbstractItemModel *model, QString str);
bool add_groups(QTreeWidget *groups);
bool add_subgroup(QTreeWidgetItem *groupItem, int groupNum);
bool get_privilege(Privileges::Type);
QPushButton* create_panel_button(QWidget *parent, QLayout *lt, QString tooltip, QString image_file, const char *method);

inline void set_columns_white_sql();
inline void set_columns_grey_sql();
inline void set_columns_white(bool prices_access);
inline void set_columns_grey(bool prices_access);
inline void set_columns_white_names();
inline void set_columns_grey_names();

QSettings *get_settings();
QSettings *get_comp_settings();
QString create_pattern(QString src, QString add_symbols = QString());

bool question(QString title, QString text);

QString replases(QString text);
void create_search_widget();
void init_vars();


bool operator ==(SortingOrder, SortingOrder);



class RightAlignmentDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    RightAlignmentDelegate(QObject *parent);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif // HELPERS_H
