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

BaseTable::~BaseTable(){
    delete settings;
}

void BaseTable::create_new_table(){
    table = new QTableView();
    switcher = new Switcher(table);
    items_on_page = ITEMS_ON_PAGE;
    current_page = 1;
    begin = 0;
    offset = 0;
    filled = false;
    settings = get_settings();
    hh = new MyHeaderView(Qt::Horizontal);
    hh->setClickable(true);
    table->setHorizontalHeader(hh);
    last_resized_index = 0;
    last_resized_width = 0;
    clist = 0;
    list = 0;

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
    QObject::connect(this->switcher, SIGNAL(page_changed(int)), SLOT(change_page(int)));

    // если получен сигнал о смене кол-ва элементов на странице, сменить значение items_on_page и послать сигнал поисковику
    QObject::connect(this->switcher, SIGNAL(items_on_page_changed(int)), SLOT(change_onpage(int)));

    // если получен сигнал singlepage_mode_selected, снять лимит записей
    QObject::connect(this->switcher, SIGNAL(singlepage_mode_selected()), SLOT(remove_limits()));

    // если получен сигнал multipage_mode_selected, установить лимит записей
    QObject::connect(this->switcher, SIGNAL(multipage_mode_selected()), SLOT(restore_limits()));

    // реакция на изменение размера столбца и его перемещение
    QObject::connect(this->table->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), SLOT(column_width_changed(int,int,int)));
    QObject::connect(this->table->horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), SLOT(column_moved(int,int,int)));

    // сохранение настроек при завершении изменения размера столбца
    QObject::connect(table->horizontalHeader(), SIGNAL(mouse_release()), SLOT(section_resized_slot()));

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
    resize_row();
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

void BaseTable::column_moved(int log, int old, int neww){
    this->save_order();
}

void BaseTable::resize_all(){
    resize_row();
}

void BaseTable::resize_row(){
    table->verticalHeader()->setDefaultSectionSize(table->verticalHeader()->fontMetrics().height() + 2);
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

void BaseTable::restore_order(){
    if(!filled) return;
    int index;
    settings->beginGroup(QString("%1/%2").arg(USERNAME).arg(settings_section));
    QMap<int,int> indexes;
    int i;
    for(i = 0; i < table->model()->columnCount(); i++){
        indexes[i] = settings->value(QString("index[%3]").arg(i), i).toInt();
    }
    settings->endGroup();
    for(i = 0; i < table->model()->columnCount(); i++){
        index = indexes.key(i);
        table->horizontalHeader()->moveSection(table->horizontalHeader()->visualIndex(index), i);
    }
}


void BaseTable::restore_width(int index, int width){
    if(index >= 0 && index < table->model()->columnCount())
        table->horizontalHeader()->resizeSection(index, width);
}

void BaseTable::hide_show_columns(){
    if(!filled) return;
    settings->beginGroup(QString("%1/%2").arg(USERNAME).arg(settings_section));
    for(int i = 0; i < table->model()->columnCount(); i++){
        //settings->value(QString("show[%1]").arg(i), "1").toString() == "1" ? table->horizontalHeader()->showSection(i) : table->horizontalHeader()->hideSection(i);
        table->setColumnHidden(i, (settings->value(QString("show[%1]").arg(i), "1").toString() == "0"));
        //table->setColumnHidden(i, false);

    }
    settings->endGroup();
}

void BaseTable::restore_columns_width(){
    if(!filled) return;
    settings->beginGroup(QString("%1/%2").arg(USERNAME).arg(settings_section));
    for(int i = 0; i < table->model()->columnCount(); i++)
        table->setColumnWidth(i, settings->value(QString("width[%1]").arg(i), DEFAULT_COLUMN_WIDTH).toInt());
    settings->endGroup();
}


void BaseTable::restore_state(){
    restore_columns_width();
    hide_show_columns();
    restore_order();
}

int BaseTable::open_columns_list(){
    clist = new QDialog(this);
    list = new QListWidget();
    // некоторые столбцы могут быть запрещены к показу! Например, складам нельзя показывать цены.
    // также не нужно показывать столбец с ед. изм. веса
    QList<int> hidden_columns;
    //hidden_columns << WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_WEIGHTUNIT);
    /*
    if(!get_privilege(Privileges::Prices_view_access)){
        hidden_columns << WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_RETAILPRICE)
                       << WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_WHOLEPRICE)
                       << WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_WHOLEBEGIN)
                       << WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_RETAILUPDATE)
                       << WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_WHOLEUPDATE)
                       << WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_WHOLEBEGINUPDATE);
    }*/
    QListWidgetItem *wi = 0;
    settings->beginGroup(QString("%1/%2").arg(USERNAME).arg(settings_section));
    for(int i = 0; i < table->model()->columnCount(); i++){
        if(hidden_columns.indexOf(i) > 0) continue;
        wi = new QListWidgetItem(original_names[column_names[i]], list);
        wi->setCheckState(settings->value(QString("show[%1]").arg(i), "1").toString() == "1" ? Qt::Checked : Qt::Unchecked);
        if(i == WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_WEIGHTUNIT)) wi->setHidden(true);
    }
    settings->endGroup();
    QWidget *btns_wgt = new QWidget();
    QPushButton *ok_button = new QPushButton("OK");
    QPushButton *cancel_button = new QPushButton("Отмена");
    QHBoxLayout *btn_layout = new QHBoxLayout();
    btn_layout->addWidget(ok_button);
    btn_layout->addWidget(cancel_button);
    ok_button->setFixedWidth(100);
    cancel_button->setFixedWidth(100);
    btns_wgt->setLayout(btn_layout);
    btns_wgt->adjustSize();
    btns_wgt->setFixedSize(btns_wgt->size());

    QVBoxLayout *lt = new QVBoxLayout();
    lt->addWidget(list);
    lt->addWidget(btns_wgt);
    clist->setLayout(lt);
    clist->adjustSize();
    //cl->setFixedSize(cl->size());

    // устанавливаем заголовок окна
    clist->setWindowTitle("Столбцы");

    // соединяем кнопки с действиями
    QObject::connect(ok_button, SIGNAL(clicked()), SLOT(accept_clist()));
    QObject::connect(cancel_button, SIGNAL(clicked()), SLOT(reject_clist()));

    int t = clist->exec();
    return t;
}

QString BaseTable::current_name(QString value){
    return original_names[value];
}

QVariant BaseTable::table_data(int column){
    if(this->table->currentIndex().row() < 0)
        return -1;
    return this->table->model()->data(this->table->model()->index(this->table->currentIndex().row(), column));
}

int BaseTable::current_row(){
    return this->table->currentIndex().row();
}

void BaseTable::set_current_row(int row){
    table->setCurrentIndex(table->model()->index(row, 0));
}

QVariant BaseTable::table_data(QString name){
    int index = -1;
    for(int i = 0; i < this->table->model()->columnCount(); i++){
        QString col = current_name(name);
        if(this->table->model()->headerData(i, Qt::Horizontal).toString() == col){
            index = i;
            break;
        }
    }
    return (index < 0 ? -1 : table_data(index));
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


//--------------------------------------------------------------------------//
//--------------------------- СЛОТЫ ----------------------------------------//
//--------------------------------------------------------------------------//

void BaseTable::change_page(int type){
    // возможные значения type описаны в constants.cpp
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
        if(switcher->page_number() >= 1 && switcher->page_number() <= total_pages){
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

void BaseTable::accept_clist(){
    settings->beginGroup(QString("%1/%2").arg(USERNAME).arg(settings_section));
    for(int i = 0; i < table->model()->columnCount(); i++){
        settings->setValue(QString("show[%1]").arg(i), list->item(i)->checkState() == Qt::Checked ? "1" : "0");
    }
    settings->endGroup();
    settings->sync();
    clist->accept();
}

void BaseTable::reject_clist(){
    clist->reject();
}

//--------------------------------------------------------------------------//
//--------------------------- СОБЫТИЯ --------------------------------------//
//--------------------------------------------------------------------------//

void BaseTable::resizeEvent(QResizeEvent *){
    // при изменении размеров таблицы необходимо корректировать положение переключателя
    move_switcher();
}

//--------------------------------------------------------------------------//
