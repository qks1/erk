#include "basetable.h"

//--------------------------------------------------------------------------//
//--------------------------- КОНСТРУКТОР-----------------------------------//
//--------------------------------------------------------------------------//

MyHeaderView::MyHeaderView(Qt::Orientation orientation, QWidget *parent) : QHeaderView(orientation, parent){
}

void MyHeaderView::mouseReleaseEvent(QMouseEvent *e){
    emit mouse_release();
    QHeaderView::mouseReleaseEvent(e);
}

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
    filled = false;
    settings = new QSettings("erk", "base");
    hh = new MyHeaderView(Qt::Horizontal);
    table->setHorizontalHeader(hh);
    last_resized_index = 0;
    last_resized_width = 0;

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
    QObject::connect(table->horizontalHeader(), SIGNAL(mouse_release()), SLOT(section_resized_slot()));
    //QObject::connect(table->horizontalHeader(), SIGNAL(mouse_release()), SLOT(save_order()));
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

    QObject::connect(this->table->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), SLOT(column_width_changed(int,int,int)));
    QObject::connect(this->table->horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), SLOT(column_moved(int,int,int)));
}

void BaseTable::section_resized_slot(){
    emit section_resized(this->last_resized_index, this->last_resized_width);
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

void BaseTable::save_order(){
    if(!filled) return;
    settings->beginGroup(QString("%1/%2").arg(USERNAME).arg(settings_section));
    for(int i = 0; i < table->model()->columnCount(); i++){
        settings->setValue(QString("index[%1]").arg(i), table->horizontalHeader()->visualIndex(i));
    }
    settings->endGroup();
    settings->sync();
    emit section_moved();
}

void BaseTable::save_state(){
    // сохраняем состояние (столбцы и их ширина)
    if(!filled) return;
    settings->beginGroup(QString("%1/%2").arg(USERNAME).arg(settings_section));
    for(int i = 0; i < table->model()->columnCount(); i++){
        // записываем в настройки ширину столбца, называя ключ оригинальным именем этого столбца
        // оригинальное имя хранится в column_names в столбце по номеру логического индекса в модели,
        // который не меняется никогда, в отличие от визуального
        settings->setValue(column_names[table->horizontalHeader()->logicalIndex(i)], table->columnWidth(table->horizontalHeader()->logicalIndex(i)));
        // записываем в настройки соответствие визуального индекса и логического,
        // чтобы потом восстановить порядок.
        // Визуальный индекс тождественен счётчику i, логический же зашит в модели.
        settings->setValue(QString(column_names[table->horizontalHeader()->logicalIndex(i)]).append("_index"), i);
    }
    settings->endGroup();
    //emit section_resized();
}

void BaseTable::restore_width(int index, int width){
    if(index >= 0 && index < table->model()->columnCount())
        table->horizontalHeader()->resizeSection(index, width);
}

void BaseTable::restore_order(){
    if(!filled) return;
    int index;
    settings->beginGroup(QString("%1/%2").arg(USERNAME).arg(settings_section));
    for(int i = 0; i < table->model()->columnCount(); i++){
        index = settings->value(QString("index[%1]").arg(i), i).toInt();
        //if(table->horizontalHeader()->visualIndex(i) != i)
          //  qDebug() << QString("logical: %1, visual: %2").arg(table->horizontalHeader()->visualIndex(i)).arg(i);
        table->horizontalHeader()->moveSection(table->horizontalHeader()->visualIndex(i), index);
    }
    settings->endGroup();
}

void BaseTable::restore_state(){
    if(!filled) return;
    for(int i = 0; i < table->model()->columnCount(); i++){
        settings->beginGroup(QString("%1/%2").arg(USERNAME).arg(settings_section));
        table->setColumnWidth(i, settings->value(QString("width[%1]").arg(i), DEFAULT_COLUMN_WIDTH).toInt());
        settings->endGroup();
    }
    restore_order();
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

void BaseTable::column_width_changed(int index, int old_width, int new_width){
    last_resized_index = index;
    last_resized_width = new_width;
}

void BaseTable::column_moved(int, int, int){
    this->save_order();
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
