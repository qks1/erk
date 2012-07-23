#include "mytablemodel.h"

MyTableModel::MyTableModel(QObject *parent) :
    QSqlQueryModel(parent)
{
}

void MyTableModel::sort(int column, Qt::SortOrder order){
    emit(order_changed(column, order));
}
