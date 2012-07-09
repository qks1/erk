#ifndef HELPERS_H
#define HELPERS_H

#include <QtGui>
#include <QtSql>

bool createConnection(QSqlDatabase);
void critical_error(QString title, QString text);
void error(QString title, QString text);

bool question(QString title, QString text);

struct pair{
    int begin;
    int end;
};

#endif // HELPERS_H
