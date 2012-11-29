#include "greytable.h"

GreyTable::GreyTable(QString settings_section,
                     bool need_switcher,
                     bool need_multiselect,
                     bool blue,
                     ColumnsStruct *columns,
                     QWidget *parent) :
    BaseTable(settings_section, need_switcher, need_multiselect, columns, parent)
{
    create_new_table();
    this->blue = blue;
    settings = get_settings();
    original_names = columns_grey_names;

    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->setAlternatingRowColors(false);
    this->setStyleSheet(blue ? "background-color: #80BBFF" : "background-color: #D4D0C8");
    filled = false;

    connects();

    restore_state();
}

GreyTable::~GreyTable(){
    delete settings;
}

void GreyTable::mousePressEvent(QMouseEvent *e){
    BaseTable::mousePressEvent(e);
}

void GreyTable::mouseReleaseEvent(QMouseEvent *e){
    BaseTable::mouseReleaseEvent(e);
}

inline void GreyTable::connects(){
    //QObject::connect(this->table->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), SLOT(header_right_click(QPoint)));
    QObject::connect(this->table, SIGNAL(customContextMenuRequested(QPoint)), SLOT(table_right_click(QPoint)));
    QObject::connect(this->table, SIGNAL(doubleClicked(QModelIndex)), SIGNAL(double_click(QModelIndex)));
}


void GreyTable::fill(GreyTableModel *query,
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
        restore_state();
    }
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


//--------------------------------------------------------------------------//
//------------------------------ МОДЕЛЬ ------------------------------------//
//--------------------------------------------------------------------------//

GreyTableModel::GreyTableModel(QWidget *parent) : MyTableModel(parent){
    background_color = QColor("#D4D0C8");
}

QVariant GreyTableModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::BackgroundRole){
        return background_color;
    }
    return MyTableModel::data(index, role);
}


//--------------------------------------------------------------------------//

BlueTableModel::BlueTableModel(QWidget *parent) : GreyTableModel(parent){

}

QVariant BlueTableModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::BackgroundRole){
        return QColor("#80FFFF");
    }
    return MyTableModel::data(index, role);
}


//--------------------------------------------------------------------------//
