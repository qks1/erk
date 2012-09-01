#include "whitetable.h"

WhiteTable::WhiteTable(QWidget *parent) :
    BaseTable(parent)
{
    create_new_table();
    settings_section = "WHITE_COLUMNS";

    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    connects();

    // восстанавливаем состояние таблицы (столбцы и их ширина)
    restore_state();
}

inline void WhiteTable::connects(){
    QObject::connect(this->table->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), SLOT(header_right_click(QPoint)));
    QObject::connect(this->table, SIGNAL(customContextMenuRequested(QPoint)), SLOT(table_right_click(QPoint)));
    QObject::connect(this->table, SIGNAL(doubleClicked(QModelIndex)), SIGNAL(double_click(QModelIndex)));
}


void WhiteTable::fill(WhiteTableModel *query,
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
    //QSortFilterProxyModel *temp_model = new QSortFilterProxyModel;
    //temp_model->setSourceModel(query);
    //temp_model->sort(0, Qt::AscendingOrder);

    this->sort_column = cur_sort_column;
    this->sort_order = cur_sort_order;

    //query->insertRow(query->rowCount());
    table->setModel(query);

    // если получен сигнал order_changed, сменить столбец сортировки и порядок
    QObject::connect(this->table->model(), SIGNAL(order_changed(int, Qt::SortOrder)),
                     this, SLOT(change_order(int, Qt::SortOrder)));


    table->sortByColumn(sort_column, sort_order);
    table->setSortingEnabled(true);
    table->horizontalHeader()->setSortIndicator(cur_sort_column, cur_sort_order);
    update_switcher_values();
    if(!filled) filled = true;
    restore_state();
}
QVariant WhiteTable::data(QModelIndex i, int column){
    return table->model()->data(table->model()->index(i.row(), column));
}

//--------------------------------------------------------------------------//
//--------------------------- СЛОТЫ ----------------------------------------//
//--------------------------------------------------------------------------//

void WhiteTable::change_order(int column, Qt::SortOrder order){
    if(!(sort_column == column && sort_order == order)){
        emit(sort_order_changed(column, order));
    }
}

void WhiteTable::header_right_click(QPoint pos){
    // проверяем, был ли щелчок произвежён по столбцу с розничной ценой
    if(this->table->horizontalHeader()->logicalIndexAt(pos) == this->column_names.indexOf("price_ret"))
        emit(prices_clicked());
    // или по столбцу с количеством
    if(this->table->horizontalHeader()->logicalIndexAt(pos) == this->column_names.indexOf("quantity"))
        emit(quantity_clicked());
}

void WhiteTable::table_right_click(QPoint pos){
    // проверяем, был ли щелчок произвежён по столбцу с розничной ценой
    int row_index = table->verticalHeader()->logicalIndexAt(pos);
    emit(right_click(table->model()->data(table->model()->index(row_index, this->column_names.indexOf("id"))).toInt()));
}


//--------------------------------------------------------------------------//
//--------------------------- СОБЫТИЯ --------------------------------------//
//--------------------------------------------------------------------------//


//--------------------------------------------------------------------------//

WhiteTableModel::WhiteTableModel(QWidget *parent) : MyTableModel(parent){
}

QVariant WhiteTableModel::data(const QModelIndex &index, int role) const{
    //if(role == Qt::DisplayRole && index.column() == 1)
        //return "";
    if(role == Qt::DecorationRole){
        if(index.column() == 1 && MyTableModel::data(index, Qt::DisplayRole).toInt() > 0)
            return QPixmap("images/photo.jpg").scaled(15,15);
    }
    if(role == Qt::DisplayRole && index.column() == 1)
        return "";


    return MyTableModel::data(index, role);
}

