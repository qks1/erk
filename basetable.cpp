#include "basetable.h"

//--------------------------------------------------------------------------//
//--------------------------- КОНСТРУКТОР-----------------------------------//
//--------------------------------------------------------------------------//

BaseTable::BaseTable(QWidget *parent) :
    QWidget(parent)
{   
}

void BaseTable::create_new_table(){
    table = new QTableView();
    switcher = new Switcher(table);
    items_on_page = ITEMS_ON_PAGE;
    current_page = 1;
    begin = 0;
    offset = 0;

    // настройки таблицы, общие для всех классов таблиц в программе
    custom_table();

    // соединения сигналов и слотов, нужные для работы переключателя
    base_connects();

    // располагаем-с
    layout();
    move_switcher();
}

//--------------------------------------------------------------------------//

inline void BaseTable::base_connects(){
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

//--------------------------------------------------------------------------//

inline void BaseTable::custom_table(){
    /*
      Устанавливаем настройки таблицы, такие как:
        -- постоянно видимые скроллбары (чтобы можно было точно спозиционировать переключатель);
        -- невидимые заголовки строк;
        -- перемещаемые столбцы;
        -- запрет редактирования;
        -- выделение целой строки;
        -- фиксированная высота строки (зависит от размера системного шрифта);
        -- отмена выделения заголовка при выделении ячейки;
        -- чередующийся цвет строк;
        -- CustomContextMenu у заголовка и таблицы, чтобы можно было назначать действия правой кнопке мыши;
    */
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
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
}

//--------------------------------------------------------------------------//
//--------------------------- ФУНКЦИИ --------------------------------------//
//--------------------------------------------------------------------------//

inline void BaseTable::layout(){
    // Просто создаём лайаут.
    // Свитчер будет позиционироваться в функции move_switcher.
    switcher->show();
    QHBoxLayout *lt = new QHBoxLayout();
    lt->addWidget(table);
    this->setLayout(lt);
}

//--------------------------------------------------------------------------//

void BaseTable::move_switcher(){
    // помещаем свитчер в правый нижний угол.
    switcher->setGeometry(table->width() - switcher->width() - table->verticalScrollBar()->width() - 5,
                          table->height() - switcher->height() - table->horizontalScrollBar()->height() - 5,
                          switcher->width(),
                          switcher->height());
}

//--------------------------------------------------------------------------//
// Действия следующих функций отражены в их названиях и пояснения не требуют.

void BaseTable::set_totals(int items){
    this->total_items = items;
    this->total_pages = (items+this->offset)/this->items_on_page + 1;
}

//--------------------------------------------------------------------------//

int BaseTable::get_items_on_page(){
    return this->items_on_page;
}

//--------------------------------------------------------------------------//

void BaseTable::set_multipage_mode(){
    this->switcher->switch_to_multipage_mode();
    update_switcher_values();
}

//--------------------------------------------------------------------------//

void BaseTable::update_switcher_values(){
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

void BaseTable::change_page(int type){
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

    // посылаем сигнал владельцу таблицы о том, что лимиты изменены
    int end = begin + items_on_page;
    pair limits = {begin, end};
    emit limits_changed(limits);
}

//--------------------------------------------------------------------------//

void BaseTable::change_onpage(int items){
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

//--------------------------------------------------------------------------//

void BaseTable::remove_limits(){
    // когда нажата кнопка "Всё на одной странице".
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

//--------------------------------------------------------------------------//

void BaseTable::restore_limits(){
    this->switcher->switch_to_multipage_mode();
    emit(limits_restored());
}

//--------------------------------------------------------------------------//
//--------------------------- СОБЫТИЯ --------------------------------------//
//--------------------------------------------------------------------------//

void BaseTable::resizeEvent(QResizeEvent *){
    // при изменении размеров таблицы необходимо корректировать положение переключателя
    move_switcher();
}

//--------------------------------------------------------------------------//
