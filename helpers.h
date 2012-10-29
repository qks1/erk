#ifndef HELPERS_H
#define HELPERS_H

#include <QtGui>
#include <QtSql>
#include "constants.h"

struct pair{
    int begin;
    int end;
};

struct SortingOrder{
    QString column;
    Qt::SortOrder order;
};

struct DetailSmall{
    int id;
    QString name;
};

bool create_connection(QSqlDatabase);
void set_system_font();
void critical_error(QString title, QString text);
void error(QString title, QString text);
QString sorting_order_to_string(QList<SortingOrder>);
int find_index(QAbstractItemModel *model, QString str);
bool add_groups(QTreeWidget *groups);
bool add_subgroup(QTreeWidgetItem *groupItem, int groupNum);
bool get_privilege(Privileges::Type);

QSettings *get_settings();
QSettings *get_comp_settings();

bool question(QString title, QString text);

QString replases(QString text);
void create_search_widget();
void init_vars();


bool operator ==(SortingOrder, SortingOrder);

#endif // HELPERS_H
