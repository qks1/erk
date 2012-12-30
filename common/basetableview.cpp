#include "basetableview.h"

BaseTableView::BaseTableView(QString settings_section,
                             bool need_multiselect,
                             ColumnsStruct *columns,
                             QList<int> default_widths,
                             QWidget *parent) :
    QTableView(parent)
{
    filled = false;
    settings = get_settings();
    this->settings_section = settings_section;
    this->need_multiselect = need_multiselect;
    this->columns = columns;
    this->default_ws = default_widths;
    resized_index = 0;
    resized_width = 0;
    old_index = 0;
    old_width = 0;
    need_moving = false;
    last_query = "";
    clist = 0;
    list = 0;

    set_header();
    custom_table();
    create_connects();
}

BaseTableView::BaseTableView(QWidget *parent) :
    QTableView(parent)
{
    filled = false;
    settings = get_settings();
    resized_index = 0;
    resized_width = 0;
    old_index = 0;
    old_width = 0;
    need_moving = false;
    last_query = "";
    clist = 0;
    list = 0;

    set_header();
    create_connects();
}

BaseTableView::~BaseTableView(){
    delete settings;
}

void BaseTableView::set_variables(QString settings_section,
                                  bool need_multiselect,
                                  ColumnsStruct *columns,
                                  QList<int> default_widths){
    this->settings_section = settings_section;
    this->need_multiselect = need_multiselect;
    this->columns = columns;
    this->default_ws = default_widths;

    custom_table();
}

inline void BaseTableView::custom_table(){
    /*
      Устанавливаем настройки таблицы, такие как:
        -- невидимые заголовки строк;
        -- перемещаемые столбцы;
        -- запрет редактирования;
        -- выделение целой строки;
        -- фиксированная высота строки (зависит от размера системного шрифта);
        -- отмена выделения заголовка при выделении ячейки;
        -- чередующийся цвет строк;
        -- CustomContextMenu у заголовка и таблицы, чтобы можно было назначать действия правой кнопке мыши;
    */
    verticalHeader()->hide();
    horizontalHeader()->setMovable(true);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(need_multiselect ? QTableView::ExtendedSelection : QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    resize_row();
    horizontalHeader()->setHighlightSections(false);
    setAlternatingRowColors(true);
    horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setHorizontalScrollMode(BaseTableView::ScrollPerItem);
}

inline void BaseTableView::create_connects(){
    // реакция на изменение размера столбца и его перемещение
    QObject::connect(this->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), SLOT(column_width_changed(int,int,int)));
    QObject::connect(this->horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), SLOT(column_moved(int,int,int)));

    // сохранение настроек при завершении изменения размера столбца
    QObject::connect(this->horizontalHeader(), SIGNAL(mouse_released()), SLOT(mouse_released_slot()));
    QObject::connect(this->horizontalHeader(), SIGNAL(mouse_pressed()), SLOT(mouse_pressed_slot()));

    //connect(this, SIGNAL(pressed(QModelIndex)), SIGNAL(clicked(QModelIndex)));
    connect(this, SIGNAL(doubleClicked(QModelIndex)), SIGNAL(double_clicked(QModelIndex)));
    connect(this, SIGNAL(mouse_pressed(QPoint)), SLOT(catch_mouse_pressed(QPoint)));
}

inline void BaseTableView::set_header(){
    hh = new SignalHeaderView(Qt::Horizontal);
    hh->setClickable(true);
    setHorizontalHeader(hh);
}

void BaseTableView::resize_all(){
    resize_row();
}

void BaseTableView::resize_row(){
    verticalHeader()->setDefaultSectionSize(verticalHeader()->fontMetrics().height() + 2);
}

void BaseTableView::set_filled(){
    filled = true;
}


/////////////////////////////////////////////////////////////////////
////////////////////// УПРАВЛЕНИЕ СТОЛБЦАМИ ////////////////////////
//////////////////////////////////////////////////////////////////

void BaseTableView::restore_width(int index, int width){
    if(model() != 0)
        if(index >= 0 && index < model()->columnCount())
            horizontalHeader()->resizeSection(index, width);
}

void BaseTableView::restore_order(int logical, int newvisual){
    if(!filled) return;
    need_moving = false;
    horizontalHeader()->moveSection(horizontalHeader()->visualIndex(logical), newvisual);
}

void BaseTableView::restore_columns_width(){
    if(!filled) return;
    // Если в настройках нет размеров столбцов, или их меньше, чем столбцлв в таблице
    // (например, это первый запуск программы или изменён файл с настройками),
    // создаём/дополняем массив columns дефолтными значениями до нужного количества.
    // Т.к. в качестве аргумента конструктора передавался УКАЗАТЕЛЬ на структуру ColumnsStruct,
    // это вызовет изменение исходной структуры, созданной в mainwindow.cpp.
    // Если этого не сделать, при записи настроек программа вылетит в трубу, то есть в segfault.
    //qDebug() << "columns: " << columns->sizes.count() << " model: " << model()->columnCount();
    if(columns->sizes.count() < model()->columnCount()){
        qDebug() << columns->sizes.count() << model()->columnCount();
        for(int i = columns->sizes.count(); i < model()->columnCount(); i++)
            columns->sizes << default_ws.at(i);
    }
    // Изменяем размер каждого столбца, устанавливая ширину, считанную из настроек и переданную в конструкторе.
    for(int i = 0; i < model()->columnCount(); i++)
        setColumnWidth(i, columns->sizes.at(i));
}


void BaseTableView::restore_columns_order(){
    if(!filled) return;
    // См. комментарий в функции restore_columns_width.
    // Отличие в том, что здесь каждому отсутствующему столбцу в качестве дефолтного значения (визуального индекса)
    // присваивается его логический индекс.
    if(columns->indices.count() < model()->columnCount()){
        for(int i = columns->indices.count(); i < model()->columnCount(); i++)
            columns->indices << i;
    }
    // Теперь устанавливаем порядок столбцов, присваивая каждому логическому индексу
    // визуальный, считанный из настроек
    // Вообще говоря, по умолчанию логический индекс должен соответствовать визуальному,
    // и функцию moveSection можно сократить до moveSection(i, columns->indices.at(i)),
    // но лучше перестрахуемся
    for(int i = 0; i < model()->columnCount(); i++)
        horizontalHeader()->moveSection(horizontalHeader()->visualIndex(i), columns->indices.at(i));
}

void BaseTableView::hide_show_columns(){
    if(!filled) return;

    // См. комментарий в функции restore_columns_width.
    // Здесь каждому отсутствующему столбцу присваивается true
    if(columns->appears.count() < model()->columnCount()){
        for(int i = columns->appears.count(); i < model()->columnCount(); i++)
            columns->appears << true;
    }

    for(int i = 0; i < model()->columnCount(); i++)
        horizontalHeader()->setSectionHidden(i, !(columns->appears.at(i)));
}


void BaseTableView::restore_state(){
    restore_columns_width();
    hide_show_columns();
    restore_columns_order();
}

int BaseTableView::open_columns_list(){
    clist = new QDialog(this);
    list = new QListWidget(this);
    QListWidgetItem *wi = 0;
    for(int i = 0; i < model()->columnCount(); i++){
        wi = new QListWidgetItem(original_names[column_names[i]], list);
        wi->setCheckState(columns->appears.at(i) ? Qt::Checked : Qt::Unchecked);
        if(i == columns_white_ids["weightunit"] || i == columns_white_ids["photo"]) wi->setHidden(true);
    }
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

void BaseTableView::accept_clist(){
    for(int i = 0; i < model()->columnCount(); i++)
        columns->appears[i] = list->item(i)->checkState() == Qt::Checked;
    clist->accept();
}

void BaseTableView::reject_clist(){
    clist->reject();
}

void BaseTableView::column_width_changed(int index, int old_width, int new_width){
    // сохраняем индекс и размер последнего изменённого столбца,
    // чтобы при отпускании кнопки мыши сработал сигнал section_resized (см. mouse_released_slot)
    resized_index = index;
    resized_width = new_width;
}

void BaseTableView::column_moved(int logical, int oldvisual, int newvisual){
    // этот слот сработает не только когда пользователь мышью перемещает столбцы,
    // но и при их перемещении функцией moveColumn, что приведёт к бесконечному циклу.
    // Для защиты от этого введена переменная need_moving, которая устанавливается в true при нажатии кнопки мыши
    // и сбрасывается в false при пользовательском перемещении столбца.
    // Таким образом, при программном перемещении столбца need_moving будет стоять в false и слот не сработает.
    if(need_moving){
        need_moving = false;
        for(int i = 0; i < columns->indices.count(); i++)
            columns->indices[i] = horizontalHeader()->visualIndex(i);
        emit section_moved(logical, newvisual, settings_section);
    }
}

void BaseTableView::mouse_pressed_slot(){
    // сообщать вышестоящему виджету о необходимости двигать столбцы надо только в том случае,
    // если это вызвано перемещением столбцов при помощи мыши.
    // Для этого устанавливаем переменную need_moving в значение true, тогда функция column_moved сможет отослать сигнал родителю
    need_moving = true;
}

void BaseTableView::mouse_released_slot(){
    // отпускание кнопки мыши не обязательно означает изменение размера столбца - может быть,
    // имело место перемещение или простой клик по заголовку.
    // При предыдущем изменении мы сохранили индекс и размер последнего изменённого столбца (переменные old_index и old_width).
    // В процессе изменения размера столбца его индекс и новый размер сохраняются в переменных resized_index и resized_width
    // (см. функцию column_width_changed). Т.о., если resized_index не совпадает с old_index или resized_width не совпадает с old_width,
    // это означает, что имело место изменение ширины столбца, о чём и сообщает родителю сигнал section_resized.
    qDebug() << columns->sizes;//.count() << " this is columns " << resized_index << " this is index";
    if(resized_index != old_index || resized_width != old_width){
        old_index = resized_index;
        old_width = resized_width;
        columns->sizes[resized_index] = resized_width;
        emit section_resized(resized_index, resized_width, settings_section);
    }
}

void BaseTableView::catch_mouse_pressed(QPoint pos){
    if(!(indexAt(pos).isValid()))
        this->setCurrentIndex(model()->index(0, 0));
}


/////////////////////////////////////////////////////////////////////
/////////////////////// УПРАВЛЕНИЕ ДАННЫМИ /////////////////////////
///////////////////////////////////////////////////////////////////


bool BaseTableView::refresh_table(){
    QSqlQueryModel *smodel = (QSqlQueryModel *)this->model();
    if(smodel == 0)
        return false;
    QString query_str = this->last_query;
    //qDebug() << query_str;
    QSqlQuery query(query_str, base);
    smodel->setQuery(query);
    return true;
}

void BaseTableView::set_last_query(QString query){
    this->last_query = query;
}

void BaseTableView::clear_last_query(){
    set_last_query(QString());
}


QString BaseTableView::current_name(QString value){
    return original_names[value];
}

QVariant BaseTableView::table_data(int column){
    if(this->model()->rowCount() == 0)
        return QVariant();

    int row;
    if(!this->currentIndex().isValid())
        row = 0;
    else
        row = this->currentIndex().row();
    return this->model()->data(this->model()->index(row, column));
}

QVariant BaseTableView::table_data(int row, int column, Qt::ItemDataRole role){
    if(this->model()->rowCount() == 0 || this->model()->columnCount() == 0)
        return QVariant();

    return this->model()->data(this->model()->index(row, column), role);
}

QVariant BaseTableView::table_multi_data(int column){
    if(this->model()->rowCount() == 0)
        return QVariant();

    QList<QVariant> lst;
    if(selectionModel()->selectedRows().count() == 0)
        lst << this->model()->data(this->model()->index(0, column));
    else foreach(QModelIndex i, selectionModel()->selectedRows()){
        lst << this->model()->data(this->model()->index(i.row(), column));
    }
    return lst;
}

int BaseTableView::selection_size(){
    return selectionModel()->selectedRows().count();
}

void BaseTableView::delete_model(){
    if(model() != 0)
        delete model();
}




void BaseTableView::mousePressEvent(QMouseEvent *event){
    if(!this->indexAt(event->pos()).isValid() && !currentIndex().isValid())
        this->setCurrentIndex(this->model()->index(0,0));
    QTableView::mousePressEvent(event);
}

void BaseTableView::mouseDoubleClickEvent(QMouseEvent *e){
    if(!this->indexAt(e->pos()).isValid()){
        if(!currentIndex().isValid())
            emit doubleClicked(this->model()->index(0,0));
        else
            emit doubleClicked(currentIndex());
    }
    else QTableView::mouseDoubleClickEvent(e);
}

void BaseTableView::wheelEvent(QWheelEvent *e){
    int delta = e->delta();
    int row = 0;
    if(this->currentIndex().isValid()){
        if(this->currentIndex().row() == 0 && delta > 0)
            return;
        if(this->currentIndex().row() == this->model()->rowCount()-1 && delta < 0)
            return;
        row = delta < 0 ? this->currentIndex().row() + 1 : this->currentIndex().row() - 1;
    }
    this->setCurrentIndex(this->model()->index(row, 0));
    scrollTo(this->model()->index(row, 0));
}

void BaseTableView::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Right){
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()+1);
    }
    if(event->key() == Qt::Key_Left){
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()-1);
    }
    else{
        QTableView::keyPressEvent(event);
    }
}



SignalHeaderView::SignalHeaderView(Qt::Orientation orientation, QWidget *parent) :
    QHeaderView(orientation, parent)
{
}

void SignalHeaderView::mouseReleaseEvent(QMouseEvent *e){
    emit mouse_released();
    QHeaderView::mouseReleaseEvent(e);
}

void SignalHeaderView::mousePressEvent(QMouseEvent *e){
    emit mouse_pressed();
    QHeaderView::mousePressEvent(e);
}

