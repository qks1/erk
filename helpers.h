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

bool createConnection(QSqlDatabase);
void set_system_font(QString family, int size);
void critical_error(QString title, QString text);
void error(QString title, QString text);
QString sorting_order_to_string(QList<SortingOrder>);
int find_index(QAbstractItemModel *model, QString str);

bool question(QString title, QString text);

QString replases(QString text);



bool operator ==(SortingOrder, SortingOrder);

#endif // HELPERS_H
