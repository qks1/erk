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
    table->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    table->setAlternatingRowColors(true);

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

    QObject::connect(this->table->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)),
                     this, SLOT(switch_prices(QPoint)));




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

void Table::fill(MyTableModel *query,
                 QStringList names,
                 int cur_sort_column,
                 Qt::SortOrder cur_sort_order,
                 int prices_filter,
                 int group,
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
    table->horizontalHeader()->setSortIndicator(table->horizontalHeader()->sortIndicatorSection(), cur_sort_order);
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
    if(!(sort_column == column && sort_order == order)){
        emit(sort_order_changed(column, order));
    }
}

void Table::switch_prices(QPoint pos){
    // проверяем, был ли щелчок произвежён по столбцу с розничной ценой
    if(this->table->horizontalHeader()->logicalIndexAt(pos) == this->column_names.indexOf("price_ret"))
        emit(prices_clicked());
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

