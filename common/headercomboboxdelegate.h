#ifndef HEADERCOMBOBOXDELEGATE_H
#define HEADERCOMBOBOXDELEGATE_H

#include <QItemDelegate>
#include <QPainter>
#include <QAbstractItemModel>
#include <QStandardItemModel>

class HeaderComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit HeaderComboBoxDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    
signals:
    
public slots:
    
};

class HeaderModel : public QStandardItemModel
{
public:
    HeaderModel(QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex & index) const;
};

#endif // HEADERCOMBOBOXDELEGATE_H
