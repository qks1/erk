#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QtGui>
#include <QtSql>

class MyTableModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit MyTableModel(QObject *parent = 0);

    void sort(int column, Qt::SortOrder order);
    
signals:
    void order_changed(int, Qt::SortOrder);
    
public slots:
    
};

#endif // MYTABLEMODEL_H
