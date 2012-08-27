#include "greytable.h"

GreyTable::GreyTable(QWidget *parent) :
    BaseTable(parent)
{
    create_new_table();
    settings = new QSettings("erk", "base");

    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->setAlternatingRowColors(false);
    this->setStyleSheet("background-color: #D4D0C8");
    filled = false;

    connects();

    // восстанавливаем состояние таблицы (столбцы и их ширина)
    //restore_state();
}

inline void GreyTable::connects(){
    //QObject::connect(this->table->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), SLOT(header_right_click(QPoint)));
    QObject::connect(this->table, SIGNAL(customContextMenuRequested(QPoint)), SLOT(table_right_click(QPoint)));
    QObject::connect(this->table->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), SLOT(column_width_changed(int,int,int)));
    QObject::connect(this->table->horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), SLOT(column_moved(int,int,int)));
    QObject::connect(this->table, SIGNAL(doubleClicked(QModelIndex)), SIGNAL(double_click(QModelIndex)));
}


void GreyTable::fill(MyTableModel *query,
                 QStringList names,
                 int cur_sort_column,
                 Qt::SortOrder cur_sort_order,
                 bool reset_page)
{
    this->column_names = names;

    if(reset_page){
        current_page = 1;
        offset = 0;
        begin = 0;
    }

    this->sort_column = cur_sort_column;
    this->sort_order = cur_sort_order;

    table->setModel(query);

    // если получен сигнал order_changed, сменить столбец сортировки и порядок
    QObject::connect(this->table->model(), SIGNAL(order_changed(int, Qt::SortOrder)),
                     this, SLOT(change_order(int, Qt::SortOrder)));


    table->sortByColumn(sort_column, sort_order);
    table->setSortingEnabled(true);
    table->horizontalHeader()->setSortIndicator(table->horizontalHeader()->sortIndicatorSection(), cur_sort_order);
    update_switcher_values();
    if(!filled){
        filled = true;
    }
    restore_state();
}

void GreyTable::save_state(){
    // сохраняем состояние (столбцы и их ширина)
    if(!filled) return;
    for(int i = 0; i < table->model()->columnCount(); i++){
        settings->beginGroup("GREY_COLUMNS");
        // записываем в настройки ширину столбца, называя ключ оригинальным именем этого столбца
        // оригинальное имя хранится в column_names в столбце по номеру логического индекса в модели,
        // который не меняется никогда, в отличие от визуального
        settings->setValue(column_names[table->horizontalHeader()->logicalIndex(i)], table->columnWidth(table->horizontalHeader()->logicalIndex(i)));
        // записываем в настройки соответствие визуального индекса и логического,
        // чтобы потом восстановить порядок.
        // Визуальный индекс тождественен счётчику i, логический же зашит в модели.
        settings->setValue(QString(column_names[table->horizontalHeader()->logicalIndex(i)]).append("_index"), i);
        settings->endGroup();
    }
}

void GreyTable::restore_state(){
    if(!filled) return;
    QString name;
    int index;
    for(int i = 0; i < table->model()->columnCount(); i++){
        settings->beginGroup("WHITE_COLUMNS");
        // Восстанавливаем ширину столбца и его визуальный индекс.
        // Ширина хранится в настройках по ключу, названному оригинальным именем столбца,
        // поэтому надо восстановить оригинальное имя.
        name = column_names[i];
        table->setColumnWidth(i, settings->value(name, DEFAULT_COLUMN_WIDTH).toInt());
        // Визуальный индекс хранится по ключу, названному оригинальным именем столбца
        // с суффиксом "_index". Восстанавливаем индекс из настроек и двигаем столбец.
        index = settings->value(QString(name).append("_index"), i).toInt();
        table->horizontalHeader()->moveSection(table->horizontalHeader()->visualIndex(i), index);
        settings->endGroup();
    }
}

void GreyTable::close_func(){
    save_state();
}

//--------------------------------------------------------------------------//
//--------------------------- СЛОТЫ ----------------------------------------//
//--------------------------------------------------------------------------//

void GreyTable::change_order(int column, Qt::SortOrder order){
    if(!(sort_column == column && sort_order == order)){
        emit(sort_order_changed(column, order));
    }
}


void GreyTable::table_right_click(QPoint pos){
    int row_index = table->verticalHeader()->logicalIndexAt(pos);
    emit(right_click(table->model()->data(table->model()->index(row_index, this->column_names.indexOf("id"))).toInt()));
}

void GreyTable::column_width_changed(int,int,int){
    save_state();
}

void GreyTable::column_moved(int, int, int){
    save_state();
}




//--------------------------------------------------------------------------//
//--------------------------- СОБЫТИЯ --------------------------------------//
//--------------------------------------------------------------------------//


//--------------------------------------------------------------------------//

