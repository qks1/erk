#include "mytablemodel.h"

MyTableModel::MyTableModel(QObject *parent) :
    QSqlQueryModel(parent)
{
}

void MyTableModel::sort(int column, Qt::SortOrder order){
    emit(order_changed(column, order));
}

QVariant MyTableModel::data (const QModelIndex &index, int role) const{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole && QSqlQueryModel::data(index).type() == QVariant::Double)
        return  QString::number(QSqlQueryModel::data(index).toDouble(), 'f', 2);

    if(role == Qt::DisplayRole && this->headerData(index.column(), Qt::Horizontal).toString() == "Год" && QSqlQueryModel::data(index).toInt() == 0 && QSqlQueryModel::data(index).toString().size() > 0)
        return QString("бг");

    return QSqlQueryModel::data(index, role);
}

Qt::ItemFlags MyTableModel::flags(const QModelIndex &index) const{
    return QSqlQueryModel::flags(index);
}
