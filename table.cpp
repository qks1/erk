#include "table.h"

Table::Table(QWidget *parent) :
    QWidget(parent)
{
    table = new QTableView();
    switcher = new Switcher(table);

    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    items_on_page = ITEMS_ON_PAGE;
    current_page = 1;
    begin = 0;
    offset = 0;



    table->verticalHeader()->hide();
    table->horizontalHeader()->setMovable(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->verticalHeader()->setDefaultSectionSize(table->verticalHeader()->fontMetrics().height() + 2);
    table->horizontalHeader()->setHighlightSections(false);



    column_names["id"] = "id";
    column_names["name"] = "Имя";
    column_names["subgroup"] = "Группа";
    column_names["price_ret"] = "Цена";
    column_names["par1_val"] = "1";
    column_names["par2_val"] = "2";
    column_names["par3_val"] = "3";
    column_names["par4_val"] = "4";
    column_names["par5_val"] = "5";
    column_names["par6_val"] = "6";
    column_names["par7_val"] = "7";
    column_names["par8_val"] = "8";
    column_names["par9_val"] = "9";
    column_names["par10_val"] = "10";
    column_names["par11_val"] = "11";
    column_names["par12_val"] = "12";


    move_switcher();
    layout();
    connects();

}

inline void Table::connects(){
    // если получен сигнал о смене номера страницы, сменить значение current_page и послать сигнал поисковику
    QObject::connect(this->switcher, SIGNAL(page_changed(int)),
                     this, SLOT(change_page(int)));

    // если получен сигнал о смене кол-ва элементов на странице, сменить значение items_on_page и послать сигнал поисковику
    QObject::connect(this->switcher, SIGNAL(items_on_page_changed(int)),
                     this, SLOT(change_onpage(int)));

    // если получен сигнал singlepage_mode_selected, снять лимит записей
    QObject::connect(this->switcher, SIGNAL(singlepage_mode_selected()),
                     this, SLOT(remove_limits()));

    // если получен сигнал multipage_mode_selected, установить лимит записей
    QObject::connect(this->switcher, SIGNAL(multipage_mode_selected()),
                     this, SLOT(restore_limits()));





}

inline void Table::layout(){
    switcher->show();
    QHBoxLayout *lt = new QHBoxLayout();
    lt->addWidget(table);
    this->setLayout(lt);
}

void Table::move_switcher(){
    switcher->setGeometry(table->width() - switcher->width() - table->verticalScrollBar()->width() - 5,
                          table->height() - switcher->height() - table->horizontalScrollBar()->height() - 5,
                          switcher->width(),
                          switcher->height());

}

QString Table::rename_column(QString str, int group){
    QRegExp rx("^(par[0-9]+)_val$");
    if(group != 0 && str.contains(rx)){
        //
        int pos = rx.indexIn(str);
        return (pos > -1 ? params_names[rx.cap(1)] : "?");
    }
    else
        return column_names[str];
}

QMap<QString, QString> Table::get_params_names(int group){
    QMap<QString, QString> params;
    QSqlQuery q;
    QString query = "SELECT * FROM " + SUBGROUPS_TABLE + " WHERE id=" + QString::number(group);
    if(!q.exec(query)){
        error("Ошибка", QString("Не удалось выполнить запрос:\n").append(query));
        // params в этом случае остаётся пустым, и названия столбцов просто не заполнятся
    }
    else {
        QSqlRecord rec = q.record();
        q.next();
        for(int i = 1; i <= MAX_PARAMS; i++){
            QString field_name = "par" + QString::number(i);
            params[field_name] = q.value(rec.indexOf(field_name)).toString();
        }
    }

    return params;
}

void Table::fill(MyTableModel *query, QString cur_sort_column, Qt::SortOrder cur_sort_order, int group, bool reset_page){
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
    qDebug("1");
    // сохраняем оригинальные имена столбцов, чтобы передавать их поисковику при сортировке
    original_column_names.clear();
    for(int i = 0; i < query->columnCount(); i++){
        original_column_names << query->headerData(i, Qt::Horizontal).toString();
    }
    int cur_index = original_column_names.indexOf(cur_sort_column);

    qDebug("2");

    // переименовываем столбцы
    // если group != 0, т.е. выбрана конкретная подгруппа, столбцы с параметрами надо переименовать параметрами для этой подгруппы.
    // для этого выбираем их названия из БД и запихиваем в QMap params_names
    if(group > 0)
        params_names = get_params_names(group);

    // и теперь для каждого столбца устанавливаем новое название
    for(int i = 0; i < query->columnCount(); i++){
        query->setHeaderData(i, Qt::Horizontal, rename_column(query->headerData(i, Qt::Horizontal).toString(), group));
    }
    qDebug("3");

    //query->insertRow(query->rowCount());
    table->setModel(query);
    qDebug("4");

    // если получен сигнал order_changed, сменить столбец сортировки и порядок
    QObject::connect(this->table->model(), SIGNAL(order_changed(int, Qt::SortOrder)),
                     this, SLOT(change_order(int, Qt::SortOrder)));

    table->setSortingEnabled(true);
    qDebug("5");

    table->horizontalHeader()->setSortIndicator(table->horizontalHeader()->sortIndicatorSection(), cur_sort_order);
    //table->sortByColumn(0, cur_sort_order);
    //table->resizeColumnsToContents();
    //table->resizeRowsToContents();

    update_switcher_values();
}


void Table::set_totals(int items){
    this->total_items = items;
    this->total_pages = (items+this->offset)/this->items_on_page + 1;
}

int Table::get_items_on_page(){
    return this->items_on_page;
}

void Table::set_multipage_mode(){
    this->switcher->switch_to_multipage_mode();
    update_switcher_values();
}

void Table::update_switcher_values(){
    this->switcher->set_current_page(this->current_page);
    this->switcher->set_total_pages(this->total_pages);
    this->switcher->set_show_all(this->total_items);
    this->switcher->set_items_on_page(this->items_on_page);
    this->switcher->set_total_items_label(this->total_items);
    move_switcher();
}

//--------------------------------------------------------------------------//
//--------------------------- СЛОТЫ ----------------------------------------//
//--------------------------------------------------------------------------//

void Table::change_page(int type){
    // возможные значения type описаны в constants.cpp
    int page;
    switch(type){
    case PAGE_FIRST:
        // если нажата кнопка "на первую страницу", устанавливаем current_page = 1 и offset = 0
        current_page = 1;
        offset = 0;
        begin = 0;
        break;
    case PAGE_PREV:
        // если нажата кнопка "на предыдущую страницу", уменьшаем current_page на 1
        // если текущая страница первая, ничего не делаем
        // если текущая страница вторая и offset не нулевой, обнуляем offset, чтобы запрос выполнился корректно и кол-во записей было верным
        if(current_page != 1){
            if(current_page == 2 && offset != 0){
                offset = 0;
                begin = 0;
                current_page = 1;
            } else {
                current_page--;
                begin -= items_on_page;
            }
        }
        break;
    case PAGE_NEXT:
        // если нажата кнопка "на следующую страницу", увеличиваем current_page на 1
        // но если текущая страница последняя, ничего не делаем
        if(current_page != total_pages){
            current_page++;
            begin += items_on_page;
        }
        break;
    case PAGE_LAST:
        // если нажата кнопка "на последнюю страницу", устанавливаем current_page = номер_последней_страницы
        current_page = total_pages;
        begin = items_on_page * (total_pages-1);
        offset = 0;
        break;
    case PAGE_NUMBER:
        // если введён номер страницы, установить current_page
        // предварительно необходимо проверить, чтобы номер был в отрезке [1, total_pages]
        page = switcher->page_number();
        if(page >= 1 && page <= total_pages){
            current_page = switcher->page_number();
            begin = items_on_page * (current_page-1);
            offset = 0;
        }
        break;
    default:
        error("Ошибка", "Ошибка при переходе на страницу №" + QString::number(current_page));
    }

    // в поисковике необходимо обновить фильтр limits
    int end = begin + items_on_page;
    pair limits = {begin, end};

    // посылаем сигнал поисковику
    emit limits_changed(limits);
}

void Table::change_onpage(int items){
    pair limits = {begin, begin+items_on_page};
    // если значение не лежит в интервале [1, total_items], то ничего не делаем
        if(items >= 1 && items <= total_items){
        // обновляем значение items_on_page
        items_on_page = items;

        // в новом отображении значение begin должно сохраниться, поэтому вычисляем offset
        offset = begin%items_on_page;

        // обновляем значение current_page
        current_page = begin/items_on_page + 1;
        if(offset != 0) current_page++;

        limits.begin = begin;
        limits.end = begin+items_on_page;
    }
    emit limits_changed(limits);
}

void Table::remove_limits(){
    // если кол-во строк больше определённого числа, запрашивать подтверждение пользователя
    if(total_items > MAX_ITEMS_WITHOUT_WARNING){
        if(question("Вы уверены?", "Вы действительно хотите показать все строки? (" + QString::number(total_items) + ")\n"
                        + "Это может занять продолжительное время.")){
            this->switcher->switch_to_singlepage_mode();
            emit(limits_removed());
        }
    }
    else
        this->switcher->switch_to_singlepage_mode();
        emit(limits_removed());
}

void Table::restore_limits(){
    this->switcher->switch_to_multipage_mode();
    emit(limits_restored());
}

void Table::change_order(int column, Qt::SortOrder order){
    QString column_name = original_column_names[column];
    if(!(sort_column == column_name && sort_order == order)){
        emit(sort_order_changed(column_name, order));
    }
}


//--------------------------------------------------------------------------//
//--------------------------- СОБЫТИЯ --------------------------------------//
//--------------------------------------------------------------------------//

void Table::resizeEvent(QResizeEvent *){
    // при изменении размеров таблицы (будь то изменение размеров окна
    // или перемещение разделителя) необходимо корректировать положение переключателя
    move_switcher();
}

//--------------------------------------------------------------------------//

