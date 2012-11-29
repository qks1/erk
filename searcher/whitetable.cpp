#include "whitetable.h"

WhiteTable::WhiteTable(QString settings_section,
                       bool need_switcher,
                       bool need_multiselect,
                       ColumnsStruct *columns,
                       QWidget *parent) :
    BaseTable(settings_section, need_switcher, need_multiselect, columns, parent)
{
    create_new_table();
    original_names = columns_white_names;

    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    connects();

    // восстанавливаем состояние таблицы (столбцы и их ширина)
    restore_state();
}

WhiteTable::~WhiteTable(){
}

inline void WhiteTable::connects(){
    QObject::connect(this->table->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), SLOT(header_right_click(QPoint)));
    QObject::connect(this->table, SIGNAL(customContextMenuRequested(QPoint)), SLOT(table_right_click(QPoint)));
    QObject::connect(this->table, SIGNAL(doubleClicked(QModelIndex)), SIGNAL(double_click(QModelIndex)));
}

void WhiteTable::restore_state(){
    restore_columns_width();
    hide_show_columns();
    restore_columns_order();
}

void WhiteTable::hide_show_columns(){
    BaseTable::hide_show_columns();
    // скрываем секции с наименованиями единиц измерения,
    // ибо они (наименования) должны отображаться в ячейке со значением (например: 5 кг)
    this->table->setColumnHidden(columns_white_ids["weightunit"], true);
    this->table->setColumnHidden(columns_white_ids["photo"], true);
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
    if(!filled){
        filled = true;
        restore_state();
    }
}

QVariant WhiteTable::data(QModelIndex i, int column){
    return table->model()->data(table->model()->index(i.row(), column));
}

void WhiteTable::set_date(){
    WhiteTableModel *model = static_cast<WhiteTableModel*>(this->table->model());
    model->set_date();
    //delete model;
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
//------------------------------- МОДЕЛЬ -----------------------------------//
//--------------------------------------------------------------------------//


//--------------------------------------------------------------------------//

WhiteTableModel::WhiteTableModel(QWidget *parent) : MyTableModel(parent){
    set_date();
    prices_access = get_privilege(Privileges::Prices_view_access);
}

void WhiteTableModel::set_date(){
    QSettings *settings = get_settings();
    retail_change_date = settings->value(QString("%1/price_change_date").arg(USERNAME), QDate::currentDate()).toDate();
    delete settings;
}

///////////////////////////////////////////////
// особенности отображения отдельных столбцов
QVariant WhiteTableModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::DecorationRole){
        // если в столбце с кол-вом сеидок по гдам положительное чило, заменияем его галочкой, инаве - посутотой, я не хочу исправять опечатки
        if(prices_access
                && index.column() == columns_white_ids["yearsdiscounts"]
                && MyTableModel::data(index, Qt::DisplayRole).toInt() > 0)
            return QPixmap(":check").scaled(qApp->font().pointSize()+5, qApp->font().pointSize()+5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        if(index.column() == columns_white_ids["reserve"]
                && MyTableModel::data(index, Qt::DisplayRole).toInt() > 0)
            return QPixmap(":check").scaled(qApp->font().pointSize()+5, qApp->font().pointSize()+5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    // если это столбец с наличием резерва, возврата или скидок (последнее только при должных полномочиях) - текст не отображаем
    if(role == Qt::DisplayRole
            && (index.column() == columns_white_ids["reserve"]
                    || (prices_access && index.column() == columns_white_ids["yearsdiscounts"])))
        return QString();

    // Единицу измерения веса приписываем к самому весу
    if(role == Qt::DisplayRole && index.column() == columns_white_ids["weight"]){
        if(MyTableModel::data(index, Qt::DisplayRole).toDouble() <= 0)
            return "";
        QString unit = MyTableModel::data(WhiteTableModel::index(index.row(), columns_white_ids["weightunit"]), Qt::DisplayRole).toString();
        return QString("%1 %2").arg(MyTableModel::data(index, Qt::DisplayRole).toString()).arg(unit);
    }

    // если есть/имеется фото - красим наименование синим
    if(role == Qt::TextColorRole && index.column() == columns_white_ids["name"])
        if(MyTableModel::data(WhiteTableModel::index(index.row(), columns_white_ids["photo"]), Qt::DisplayRole).toString().length() > 0){
            return Qt::blue;
    }

    if(prices_access){
        // для розничной цены - если дата обновления больше retail_change_date, красим её красным
        if(role == Qt::TextColorRole && index.column() == columns_white_ids["price_ret"])
            if(QDate::fromString(MyTableModel::data(WhiteTableModel::index(index.row(), columns_white_ids["retailupdate"]), Qt::DisplayRole).toString(), "yyyy-MM-dd") >= retail_change_date){
                return Qt::red;
        }
        // аналогично - для оптовой и для "опт с"
        if(role == Qt::TextColorRole && index.column() == columns_white_ids["price_whole"])
            if(QDate::fromString(MyTableModel::data(WhiteTableModel::index(index.row(), columns_white_ids["wholeupdate"]), Qt::DisplayRole).toString(), "yyyy-MM-dd") >= retail_change_date){
                return Qt::red;
        }
        if(role == Qt::TextColorRole && index.column() == columns_white_ids["whole_begin"])
            if(QDate::fromString(MyTableModel::data(WhiteTableModel::index(index.row(), columns_white_ids["wholebeginupdate"]), Qt::DisplayRole).toString(), "yyyy-MM-dd") >= retail_change_date){
                return Qt::red;
        }

        // если в столбце с датой изменения цены (любом из трёх) стоит фиктивная дата (1900-01-01) - отображаем пустоту
        if(role == Qt::DisplayRole && (index.column() == columns_white_ids["retailupdate"] ||
                                       index.column() == columns_white_ids["wholeupdate"] ||
                                       index.column() == columns_white_ids["wholebeginupdate"])){
            if(MyTableModel::data(index, Qt::DisplayRole).toString() == "1900-01-01")
                return "";
        }

        // если в cтолбце "опт с" стоит ноль (то есть значение не указано) - не отображаем ничего
        if(role == Qt::DisplayRole && index.column() == columns_white_ids["whole_begin"]){
            if(MyTableModel::data(index, Qt::DisplayRole).toInt() <= 0)
                return "";
        }
    }

    // в остальных случаях оставляем данные без изменений
    return MyTableModel::data(index, role);
}

QVariant WhiteTableModel::headerData(int section, Qt::Orientation orientation, int role) const{
    //if(MyTableModel::headerData(section, orientation).toString() == "Фото")
      //  return "";

    return MyTableModel::headerData(section, orientation, role);
}

void WhiteTableModel::sort(int column, Qt::SortOrder order){
    MyTableModel::sort(column, order);
}

