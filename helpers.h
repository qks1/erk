#ifndef HELPERS_H
#define HELPERS_H

#include <QtGui>
#include <QtSql>

bool createConnection(QSqlDatabase);
void critical_error(QString title, QString text);
void error(QString title, QString text);

#endif // HELPERS_H
