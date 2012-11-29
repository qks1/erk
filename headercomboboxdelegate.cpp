#include "headercomboboxdelegate.h"

HeaderComboBoxDelegate::HeaderComboBoxDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void HeaderComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    // Current value text
    QString valueString = index.model()->data(index, Qt::DisplayRole).toString();

    // If item is disabled draw it as header item
    if(!(index.model()->flags(index) & Qt::ItemIsEnabled)){
        // Make italic font like in html
        QFont *font = new QFont();
        font->setItalic(true);
        painter->setFont(*font);

        // Fill background grey color
        QPalette::ColorGroup cg = QPalette::Disabled;
        cg = QPalette::Inactive;
        painter->fillRect(option.rect, option.palette.brush(cg, QPalette::Highlight));

        // Draw it
        painter->drawText(option.rect, Qt::AlignLeft | Qt::TextSingleLine, valueString);

    }
    else
    QItemDelegate::paint (painter, option, index);
}

HeaderModel::HeaderModel(QObject *parent) : QStandardItemModel(parent)
{
}

Qt::ItemFlags HeaderModel::flags(const QModelIndex &index) const{
    Qt::ItemFlags flags = QStandardItemModel::flags(index);

    if (!index.isValid())
        return flags;

    int value =	index.data(Qt::UserRole).toInt();
    if (value == -1){
        flags &= ~Qt::ItemIsSelectable;
        flags &= ~Qt::ItemIsEnabled;
    }

    return flags;
 }
