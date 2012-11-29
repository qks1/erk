#include "basetable.h"

//--------------------------------------------------------------------------//
//--------------------------- КОНСТРУКТОР-----------------------------------//
//--------------------------------------------------------------------------//

MyHeaderView::MyHeaderView(Qt::Orientation orientation, QWidget *parent) : QHeaderView(orientation, parent){
}

void MyHeaderView::mouseReleaseEvent(QMouseEvent *e){
    emit mouse_released();
    QHeaderView::mouseReleaseEvent(e);
}

void MyHeaderView::mousePressEvent(QMouseEvent *e){
    emit mouse_pressed();
    QHeaderView::mousePressEvent(e);
}

void MyHeaderView::paintEvent(QPaintEvent *e){
    QHeaderView::paintEvent(e);
    //QPainter painter(this);
    //painter.setPen(Qt::black);
    //painter.drawLine(0,0,width(),height());
}

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
 ///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

SmartTableView::SmartTableView(QWidget *parent) : QTableView(parent){
}

void SmartTableView::mousePressEvent(QMouseEvent *event){
    //emit mouse_pressed(event->pos());
    if(!this->indexAt(event->pos()).isValid())
        this->setCurrentIndex(this->model()->index(0,0));
    QTableView::mousePressEvent(event);
}

void SmartTableView::mouseReleaseEvent(QMouseEvent *event){
}

void SmartTableView::mouseDoubleClickEvent(QMouseEvent *e){
    emit doubleClicked(this->indexAt(e->pos()));
    QTableView::mouseDoubleClickEvent(e);
}

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
 ///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////


BaseTable::BaseTable(QString settings_section,
                     bool need_switcher,
                     bool need_multiselect,
                     ColumnsStruct *columns,
                     QWidget *parent) :
    QWidget(parent)
{   
    this->settings_section = settings_section;
    this->need_switcher = need_switcher;
    this->need_multiselect = need_multiselect;
    this->columns = columns;
    resized_index = 0;
    resized_width = 0;
    old_index = 0;
    old_width = 0;
    need_moving = false;
    last_query = "";
}

void BaseTable::paintEvent(QPaintEvent *e){
    QWidget::paintEvent(e);
}

BaseTable::~BaseTable(){
    delete settings;
}

void BaseTable::mouseReleaseEvent(QMouseEvent *e){
    //qDebug() << table->indexAt(e->pos()).column();
    QWidget::mouseReleaseEvent(e);
}

void BaseTable::create_new_table(){
    table = new SmartTableView();
    //switcher = (need_switcher ? new Switcher(this) : 0);
    switcher = new Switcher(this);
    items_on_page = ITEMS_ON_PAGE;
    current_page = 1;
    total_pages = 0;
    total_items = 0;
    begin = 0;
    offset = 0;
    filled = false;
    settings = get_settings();

    // устанавливаем наш умный заголовок, который умеет отсылать сигналы таблице
    hh = new MyHeaderView(Qt::Horizontal);
    hh->setClickable(true);
    table->setHorizontalHeader(hh);
    resized_index = 0;
    resized_width = 0;
    clist = 0;
    list = 0;

    // настройки таблицы, общие для всех классов таблиц в программе
    custom_table();

    // соединения сигналов и слотов, нужные для работы переключателя
    base_connects();
    connect(table, SIGNAL(mouse_pressed(QPoint)), SLOT(catch_mouse_pressed(QPoint)));

    // располагаем-с
    layout();
    ///////if(need_switcher)
        move_switcher();


}

//--------------------------------------------------------------------------//

inline void BaseTable::base_connects(){
    /////if(need_switcher){
        // если получен сигнал о смене номера страницы, сменить значение current_page и послать сигнал поисковику
        QObject::connect(this->switcher, SIGNAL(page_changed(int)), SLOT(change_page(int)));

        // если получен сигнал о смене кол-ва элементов на странице, сменить значение items_on_page и послать сигнал поисковику
        QObject::connect(this->switcher, SIGNAL(items_on_page_changed(int)), SLOT(change_onpage(int)));

        // если получен сигнал singlepage_mode_selected, снять лимит записей
        QObject::connect(this->switcher, SIGNAL(singlepage_mode_selected()), SLOT(remove_limits()));

        // если получен сигнал multipage_mode_selected, установить лимит записей
        QObject::connect(this->switcher, SIGNAL(multipage_mode_selected()), SLOT(restore_limits()));
    /////}

    // реакция на изменение размера столбца и его перемещение
    QObject::connect(this->table->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), SLOT(column_width_changed(int,int,int)));
    QObject::connect(this->table->horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), SLOT(column_moved(int,int,int)));

    // сохранение настроек при завершении изменения размера столбца
    QObject::connect(table->horizontalHeader(), SIGNAL(mouse_released()), SLOT(mouse_released_slot()));
    QObject::connect(table->horizontalHeader(), SIGNAL(mouse_pressed()), SLOT(mouse_pressed_slot()));

}

void BaseTable::set_last_query(QString query){
    this->last_query = query;
}

void BaseTable::clear_last_query(){
    set_last_query(QString());
}

void BaseTable::catch_mouse_pressed(QPoint pos){
    if(!(table->indexAt(pos).isValid()))
        table->setCurrentIndex(table->model()->index(0, 0));
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
    table->setSelectionMode(need_multiselect ? QTableView::ExtendedSelection : QAbstractItemView::SingleSelection);
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
    // Свитчер будет позиционироваться в функции move_switcher, если он нужен.
    /*if(need_switcher) */switcher->show();
    QHBoxLayout *lt = new QHBoxLayout();
    lt->addWidget(table);
    lt->setMargin(0);
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

void BaseTable::resize_all(){
    resize_row();
}

void BaseTable::resize_row(){
    table->verticalHeader()->setDefaultSectionSize(table->verticalHeader()->fontMetrics().height() + 2);
    //table->setRowHeight(table->font().pointSize() + 2);
    //table->resizeRowsToContents();
}

bool BaseTable::refresh_table(){
    QSqlQueryModel *model = (QSqlQueryModel *)table->model();
    if(model == 0)
        return false;
    QString query_str = this->last_query;
    //qDebug() << query_str;
    QSqlQuery query(query_str, base);
    model->setQuery(query);
    return true;
}



void BaseTable::restore_order(int logical, int newvisual){
    if(!filled) return;
    need_moving = false;
    table->horizontalHeader()->moveSection(table->horizontalHeader()->visualIndex(logical), newvisual);
}


void BaseTable::restore_width(int index, int width){
    if(table->model() != 0)
        if(index >= 0 && index < table->model()->columnCount())
            table->horizontalHeader()->resizeSection(index, width);
}

void BaseTable::delete_model(){
    if(table->model() != 0)
        delete table->model();
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
    // Если в настройках нет размеров столбцов, или их меньше, чем столбцлв в таблице
    // (например, это первый запуск программы или изменён файл с настройками),
    // создаём/дополняем массив columns дефолтными значениями до нужного количества.
    // Т.к. в качестве аргумента конструктора передавался УКАЗАТЕЛЬ на структуру ColumnsStruct,
    // это вызовет изменение исходной структуры, созданной в mainwindow.cpp.
    // Если этого не сделать, при записи настроек программа вылетит в трубу, то есть в segfault.
    if(columns->sizes.count() < table->model()->columnCount()){
        for(int i = columns->sizes.count(); i < table->model()->columnCount(); i++)
            columns->sizes << DEFAULT_COLUMN_WIDTH;
    }
    // Изменяем размер каждого столбца, устанавливая ширину, считанную из настроек и переданную в конструкторе.
    for(int i = 0; i < table->model()->columnCount(); i++)
        table->setColumnWidth(i, columns->sizes.at(i));
}


void BaseTable::restore_columns_order(){
    if(!filled) return;
    // См. комментарий в функции restore_columns_width.
    // Отличие в том, что здесь каждому отсутствующему столбцу в качестве дефолтного значения (визуального индекса)
    // присваивается его логический индекс.
    if(columns->indices.count() < table->model()->columnCount()){
        for(int i = columns->indices.count(); i < table->model()->columnCount(); i++)
            columns->indices << i;
    }
    // Теперь устанавливаем порядок столбцов, присваивая каждому логическому индексу
    // визуальный, считанный из настроек
    // Вообще говоря, по умолчанию логический индекс должен соответствовать визуальному,
    // и функцию moveSection можно сократить до moveSection(i, columns->indices.at(i)),
    // но лучше перестрахуемся
    for(int i = 0; i < table->model()->columnCount(); i++)
        table->horizontalHeader()->moveSection(table->horizontalHeader()->visualIndex(i), columns->indices.at(i));
    /*
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
    }*/
}

void BaseTable::restore_state(){
    restore_columns_width();
    hide_show_columns();
    restore_columns_order();
}

int BaseTable::open_columns_list(){
    clist = new QDialog(this);
    list = new QListWidget(this);
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
        if(i == columns_white_ids["weightunit"]) wi->setHidden(true);
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

QVariant BaseTable::table_data(int column){
    if(this->table->model()->rowCount() == 0)
        return QVariant();

    int row;
    if(!this->table->currentIndex().isValid())
        row = 0;
    else
        row = this->table->currentIndex().row();
    return this->table->model()->data(this->table->model()->index(row, column));
}

QVariant BaseTable::table_multi_data(int column){
    if(this->table->model()->rowCount() == 0)
        return QVariant();

    QList<QVariant> lst;
    if(table->selectionModel()->selectedRows().count() == 0)
        lst << this->table->model()->data(this->table->model()->index(0, column));
    else foreach(QModelIndex i, table->selectionModel()->selectedRows()){
        lst << this->table->model()->data(this->table->model()->index(i.row(), column));
    }
    return lst;
}

int BaseTable::current_row(){
    return this->table->currentIndex().row();
}

void BaseTable::set_current_row(int row){
    if(table->model() != 0)
        table->setCurrentIndex(table->model()->index(row, 0));
}

int BaseTable::row_count(){
    return table->model()->rowCount();
}

int BaseTable::selection_size(){
    return table->selectionModel()->selectedRows().count();
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
    if(need_switcher){
        this->switcher->switch_to_multipage_mode();
        update_switcher_values();
    }
}

//--------------------------------------------------------------------------//

void BaseTable::update_switcher_values(){
    if(need_switcher){
        this->switcher->set_current_page(this->current_page);
        this->switcher->set_total_pages(this->total_pages);
        this->switcher->set_show_all(this->total_items);
        this->switcher->set_items_on_page(this->items_on_page);
        this->switcher->set_total_items_label(this->total_items);
        move_switcher();
    }
}

void BaseTable::column_width_changed(int index, int old_width, int new_width){
    // сохраняем индекс и размер последнего изменённого столбца,
    // чтобы при отпускании кнопки мыши сработал сигнал section_resized (см. mouse_released_slot)
    resized_index = index;
    resized_width = new_width;
}

void BaseTable::column_moved(int logical, int oldvisual, int newvisual){
    // этот слот сработает не только когда пользователь мышью перемещает столбцы,
    // но и при их перемещении функцией moveColumn, что приведёт к бесконечному циклу.
    // Для защиты от этого введена переменная need_moving, которая устанавливается в true при нажатии кнопки мыши
    // и сбрасывается в false при пользовательском перемещении столбца.
    // Таким образом, при программном перемещении столбца need_moving будет стоять в false и слот не сработает.
    if(need_moving){
        need_moving = false;
        for(int i = 0; i < columns->indices.count(); i++)
            columns->indices[i] = table->horizontalHeader()->visualIndex(i);
        emit section_moved(logical, newvisual, settings_section);
    }
}

void BaseTable::mouse_pressed_slot(){
    // сообщать вышестоящему виджету о необходимости двигать столбцы надо только в том случае,
    // если это вызвано перемещением столбцов при помощи мыши.
    // Для этого устанавливаем переменную need_moving в значение true, тогда функция column_moved сможет отослать сигнал родителю
    need_moving = true;
}

void BaseTable::mouse_released_slot(){
    // отпускание кнопки мыши не обязательно означает изменение размера столбца - может быть,
    // имело место перемещение или простой клик по заголовку.
    // При предыдущем изменении мы сохранили индекс и размер последнего изменённого столбца (переменные old_index и old_width).
    // В процессе изменения размера столбца его индекс и новый размер сохраняются в переменных resized_index и resized_width
    // (см. функцию column_width_changed). Т.о., если resized_index не совпадает с old_index или resized_width не совпадает с old_width,
    // это означает, что имело место изменение ширины столбца, о чём и сообщает родителю сигнал section_resized.
    if(resized_index != old_index || resized_width != old_width){
        old_index = resized_index;
        old_width = resized_width;
        emit section_resized(resized_index, resized_width, settings_section);
    }
}


//--------------------------------------------------------------------------//
//--------------------------- СЛОТЫ ----------------------------------------//
//--------------------------------------------------------------------------//

void BaseTable::change_page(int type){
    if(!need_switcher)
        return;

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
    if(!need_switcher)
        return;

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
    if(!need_switcher)
        return;

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
    if(!need_switcher)
        return;

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
    /*if(need_switcher)*/ move_switcher();
}

void BaseTable::wheelEvent(QWheelEvent *e){
    QWidget::wheelEvent(e);
}

//--------------------------------------------------------------------------//
