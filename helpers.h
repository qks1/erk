#ifndef HELPERS_H
#define HELPERS_H

#include <QtGui>
#include <QtSql>
#include "constants.h"

bool createConnection(QSqlDatabase);
void set_system_font(QString family, int size);
void critical_error(QString title, QString text);
void error(QString title, QString text);

bool question(QString title, QString text);

QString replases(QString text);

struct pair{
    int begin;
    int end;
};

struct SortingOrder{
    QString column;
    Qt::SortOrder order;
};

bool operator ==(SortingOrder, SortingOrder);

#endif // HELPERS_H
