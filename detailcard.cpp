#include "detailcard.h"

DetailCard::DetailCard(int id, QWidget *parent) :
    QWidget(parent)
{
    this->id = id;
    this->parent = parent;
    //exec();
}

int DetailCard::exec(){
    QDialog *info = new QDialog(parent);

    QLabel *info_label = new QLabel();
    QLabel *quantity_label = new QLabel();
    // выбираем название, группу, подгруппу, общее кол-во
    QString tm_query = QString("SELECT t.name as trademark, t.quantity, s.name as subgroup, g.name as group") +
            " FROM trademarks t " +
            " JOIN subgroups s ON (t.subgroup_id = s.id) " +
            " JOIN groups g ON (g.id=s.group_id) " +
            " WHERE t.id = " + QString::number(id);
    QSqlQuery *sqlquery = new QSqlQuery(tm_query);
    if(sqlquery->lastError().isValid()){
        error("Ошибка", sqlquery->lastError().text());
    }
    QSqlRecord rec = sqlquery->record();
    sqlquery->next();
    QString trademark = sqlquery->value(rec.indexOf("trademark")).toString();
    QString subgroup = sqlquery->value(rec.indexOf("subgroup")).toString();
    QString group = sqlquery->value(rec.indexOf("group")).toString();
    int quantity = sqlquery->value(rec.indexOf("quantity")).toInt();

    info_label->setText(QString("<h2>%1</h2><br /><h3>%2, %3</h3><br />").arg(trademark).arg(group).arg(subgroup));
    quantity_label->setText(QString("<h4>Общее кол-во на складе: %1").arg(quantity));


    // формирование таблицы

    QString query = QString("SELECT g.id, t.name, g.quantity, g.year, p.storage, p.rack, p.board, p.box, g.inspection, g.add_info, g.defect, c.category_name ") +
                        " FROM greytable g " +
                        " JOIN trademarks t ON (t.id=g.trademark_id) " +
                        " JOIN places p ON (p.id=g.place_id) " +
                        " JOIN categories c ON (g.category_id=c.id) " +
                        " WHERE t.id = " + QString::number(id);
    MyTableModel *model = new MyTableModel();
    model->setQuery(query);
    if(model->lastError().isValid()){
        error("Ошибка", model->lastError().text());
    }

    // переименование столбцов
    for(int i = 0; i < model->columnCount(); i++){
        model->setHeaderData(i, Qt::Horizontal, GREY_COLUMNS_NAMES[model->headerData(i, Qt::Horizontal).toString()]);
    }

    QTableView *table = new QTableView();
    table->setModel(model);
    table->verticalHeader()->hide();
    table->horizontalHeader()->setMovable(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->verticalHeader()->setDefaultSectionSize(table->verticalHeader()->fontMetrics().height() + 2);
    table->horizontalHeader()->setHighlightSections(false);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    table->setContextMenuPolicy(Qt::CustomContextMenu);

    // переименование столбцов
    //for(int i = 0; i < model->columnCount(); i++)
        //model->setHeaderData(i, Qt::Horizontal, GREY_COLUMNS_NAMES[model->headerData(i, Qt::Horizontal).toString()]);


    table->resizeColumnsToContents();

    // расположение элементов

    QVBoxLayout *vlt = new QVBoxLayout();
    vlt->addWidget(info_label);
    vlt->addWidget(quantity_label);
    vlt->addWidget(table);

    info->setLayout(vlt);
    info->resize(600,300);
    info->setWindowTitle("Карточка детали");
    return info->exec();
}
