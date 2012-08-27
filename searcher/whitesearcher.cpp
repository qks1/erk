#include "whitesearcher.h"

WhiteSearcher::WhiteSearcher(QWidget *parent) :
    QWidget(parent)
{
    // создаём виджеты
    white_table = new WhiteTable(this);
    catalog = new Catalog();
    input = new Input();
    ParamsSelector *param = 0;
    for(int i = 0; i < MAX_PARAMS; i++){
        param = new ParamsSelector(i);
        selectors << param;
    }
    params_reset = new QPushButton("X");

    // заполняем переменные
    single_group = -1;
    single_group_without_limits = -1;
    boxes_filled = false;

    // размещаем все элементы
    set_layout();

    // соединяем сигналы со слотами
    white_connects();
}

inline void WhiteSearcher::white_connects(){
    // смена группы
    QObject::connect(this->catalog, SIGNAL(group_changed(int)), SIGNAL(group_changed(int)));
    // смена пределов
    QObject::connect(this->white_table, SIGNAL(limits_changed(pair)), SIGNAL(limits_changed(pair)));
    // снятие лимита
    QObject::connect(this->white_table, SIGNAL(limits_removed()), SIGNAL(limits_removed()));
    // установка лимита
    QObject::connect(this->white_table, SIGNAL(limits_restored()), SIGNAL(limits_restored()));
    // изменение текста
    QObject::connect(this->input, SIGNAL(text_changed_signal(int,QString)), SLOT(text_changed_slot(int,QString)));
    // очистка текста
    QObject::connect(this->input, SIGNAL(reset_signal()), SIGNAL(reset_signal()));
    // изменение порядка сортировки
    QObject::connect(this->white_table, SIGNAL(sort_order_changed(int,Qt::SortOrder)), SIGNAL(sort_order_changed(int,Qt::SortOrder)));
    for(int i = 0; i < selectors.size(); i++){
        // заполнить боксы
        QObject::connect(this->selectors.at(i), SIGNAL(fill_boxes_signal()), SLOT(fill_boxes_slot()));
        // выбор элемента из селектора
        QObject::connect(this->selectors.at(i), SIGNAL(item_selected(QString,int)), SIGNAL(item_selected(QString,int)));
        // нажатие на кнопку над селектором
        QObject::connect(this->selectors.at(i), SIGNAL(button_clicked(int)), SIGNAL(button_clicked(int)));
    }
    // сброс селекторов
    QObject::connect(this->params_reset, SIGNAL(clicked()), SIGNAL(params_reset_clicked()));
    // изменение фильтра по цене
    QObject::connect(this->white_table, SIGNAL(prices_clicked()), SIGNAL(prices_clicked()));
    // изменение фильтра по кол-ву
    QObject::connect(this->white_table, SIGNAL(quantity_clicked()), SIGNAL(quantity_clicked()));
    // открыть карточку детали
    QObject::connect(this->white_table, SIGNAL(right_click(int)), SLOT(detail_info(int)));
    // открыть серый экран
    QObject::connect(this->white_table, SIGNAL(double_click(QModelIndex)), SLOT(open_grey(QModelIndex)));
}

inline void WhiteSearcher::set_layout(){
    // виджет, содержащий кнопки и текстовое поле
    QWidget *top_panel = new QWidget(this);
    // виджет, содержащий выпадающие списки параметров
    QWidget *selectors_panel = new QWidget(this);
    // на эти два виджета будут наложены горизонтальные лайауты

    // устанавливаем размеры элементов
    int input_width = 300;      // ширина текстового поля
    int button_size = 25;       // ширина кнопки сброса
    int spacing = 5;            // отступ до кнопки сброса

    // поправка на ширину боковой панели в бубунте. Осторожно, костыль.
    int panel_width = 0;
#ifdef Q_OS_LINUX
    panel_width = 100;
#endif
    // да и это тоже костыль. Ширина селектора при максимизированном окне.
    int selector_width = (qApp->desktop()->width() - button_size - panel_width - spacing)/MAX_PARAMS;

    // лайаут для виджета с кнопками и текстовым полем
    QHBoxLayout *top_lt = new QHBoxLayout();
    top_lt->setMargin(0);
    top_lt->setSpacing(0);
    // добавляем кнопки, которые пока не созданы, лол
    // блаблабла
    // добавляем текстовое поле
    top_lt->addWidget(input);
    input->setFixedSize(input_width, 70);
    top_panel->setLayout(top_lt);
    top_panel->setFixedHeight(input->height());

    // лайаут для виджета с селекторами
    QHBoxLayout *wlt = new QHBoxLayout;
    wlt->setSpacing(0);
    wlt->setMargin(0);
    for(int i = 0; i < MAX_PARAMS; i++){
        selectors.at(i)->setFixedWidth(selector_width);
        selectors.at(i)->setGeometry(selector_width*i,0,selector_width,selectors.at(i)->height());
        selectors.at(i)->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        wlt->addWidget(selectors.at(i));
    }
    wlt->addSpacing(spacing);
    wlt->addWidget(params_reset);
    params_reset->setFixedSize(button_size, button_size);
    selectors_panel->setLayout(wlt);
    selectors_panel->setMinimumWidth(500);
    selectors_panel->setFixedHeight(selectors.at(0)->height());

    // формируем разделитель, содержащий каталог и таблицу
    QSplitter *splitter = split();

    // главный layout
    QVBoxLayout *mainlt= new QVBoxLayout;
    mainlt->addWidget(top_panel);
    mainlt->addWidget(selectors_panel);
    mainlt->addWidget(splitter);

    this->setLayout(mainlt);
}

QSplitter* WhiteSearcher::split(){
    // формируем разделитель, содержащий каталог и таблицу
    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(this->catalog);
    splitter->addWidget(this->white_table);

    // устанавливаем соотношение размеров 1:4
    QList<int> sz;
    sz << 100 << 400;
    splitter->setSizes(sz);

    return splitter;
}

QMap<QString, QString> WhiteSearcher::get_params_names(int group){
    // получить список названий параметров для данной подгруппы
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


QString WhiteSearcher::rename_column(QString str, int group){
    // Если группа определена, то столбцы с параметрами
    // обзываем названиями этих параметров.
    // Иначе берём название из WHITE_COLUMNS_NAMES.
    QRegExp rx("^(par[0-9]+)_val$");
    if(group > 0 && str.contains(rx)){
        int pos = rx.indexIn(str);
        return (pos > -1 ? params_names[rx.cap(1)] : "?");
    }
    else
        return WHITE_COLUMNS_NAMES[str];
}


void WhiteSearcher::set_button_labels(){
    // устанавливаем названия кнопок над селекторами.
    // Собственно, они уже хранятся в params_names, хорошо бы ещё прибавить количество
    // если single_group_without_limits > 0, то кнопки будут называться
    // именами параметров для этой подгруппы, иначе - номерами.
    QString str;
    int cnt;
    for(int i = 0; i < MAX_PARAMS; i++){
        str = (single_group_without_limits > 0 ? params_names["par" + QString::number(i+1)] : QString::number(i+1));
        cnt = selectors.at(i)->count();
        selectors.at(i)->set_button_label(str, cnt > 0 ? cnt-1 : cnt);
    }
}

void WhiteSearcher::set_button_labels(QSqlQuery query){
    query.setForwardOnly(true);
    query.next();
    QString str;
    int cnt;
    for(int i = 0; i < MAX_PARAMS; i++){
        str = (single_group_without_limits > 0 ? params_names["par" + QString::number(i+1)] : QString::number(i+1));
        cnt = query.value(0).toInt();
        query.next();
        selectors.at(i)->set_button_label(str, cnt > 0 ? cnt-1 : cnt);
    }
}

void WhiteSearcher::set_text_button_labels(){
    /*QSqlQuery q;
    int cnt;
    for(int i = 0; i < MAX_PARAMS; i++){
        q.exec("SELECT DISTINCT t.par" + QString::number(i+1) + "_val " + tables_string + where_string);
        if(q.lastError().isValid()){
            error("Ошибка при подсчёте параметров", q.lastError().text());
            cnt = 0;
        }
        else cnt = q.size();

        selectors.at(i)->set_button_label(single_group_without_limits > 0 ? params_names["par" + QString::number(i+1)] : QString::number(i+1), cnt);
    }*/
    for(int i = 0; i < MAX_PARAMS; i++)
        selectors.at(i)->set_button_label(single_group_without_limits > 0 ? params_names["par" + QString::number(i+1)] : QString::number(i+1));
}

void WhiteSearcher::clear_button_labels(){
    // очищаем названия кнопок над селекторами.
    for(int i = 0; i < MAX_PARAMS; i++){
        selectors.at(i)->clear_button_label();
    }
}

int WhiteSearcher::set_totals(QString query){
    QSqlQuery sql;
    int total;
    if(!sql.exec(query)){
        error("Ошибка", QString("Не удалось выполнить запрос:\n").append(query));
        total = -1;
    } else {
        sql.next();
        total = sql.value(0).toInt();
    }
    return total;
}


void WhiteSearcher::get_params_list(){
    QString query_str, par_name;
    QStringList list;
    QSqlQueryModel* query = 0;
    // сформируем списки параметров деталей
    // для этого в классе есть лист двумерный QStringList, куда и будем их складывать
    for(int i = 0; i < MAX_PARAMS; i++){
        query = new QSqlQueryModel();
        par_name = "t.par" + QString::number(i+1) + "_val";
        list.clear();
        selectors.at(i)->clear_items();
        query_str = "(SELECT '') UNION (SELECT " + par_name + tables_string + where_string + " AND " + par_name + "!= '')  ORDER BY 1";
        query->setQuery(query_str);
        //qDebug() << query_str;
        selectors.at(i)->setModel(query);
        selectors.at(i)->setEnabled(!(selectors.at(i)->count() <= 1 && selectors.at(i)->item(0) == ""));
        if(selectors.at(i)->count() == 2 && selectors.at(i)->item(0) == "")
            selectors.at(i)->set_selected(1);
    }
    delete query;
    set_button_labels();
}

void WhiteSearcher::detail_info(int id){
    DetailCard card(id, this);
    card.exec();
}

void WhiteSearcher::clear_text(){
    this->input->set_text("");
}

int WhiteSearcher::input_id_combobox_value(){
    return this->input->id_combobox_value();
}

int WhiteSearcher::get_original_index(QString name){
    return original_column_names.indexOf(name);
}

QString WhiteSearcher::get_original_name(int index){
    return original_column_names[index];
}

void WhiteSearcher::fill_boxes_slot(){
    if(!boxes_filled){
        get_params_list();
        boxes_filled = true;
    }
}

void WhiteSearcher::text_changed_slot(int type, QString text){
    // очистить селекторы
    for(int i = 0; i < MAX_PARAMS; i++){
        selectors.at(i)->set_edit_text("");
        selectors.at(i)->clear_items();
    }
    emit text_changed_signal(WHITE_MODE, type, text);
}

void WhiteSearcher::open_grey(QModelIndex i){
    //QMessageBox::warning(this, "qq", white_table->data(i, 1).toString());
    int id = white_table->data(i, 0).toInt();
    emit(create_grey(id));
}

void WhiteSearcher::close_func(){
    this->white_table->close_func();
}

void WhiteSearcher::fill_table(Filters *filters, bool reset_groups = false, bool delete_last_symbol = false){
    MyTableModel *query = new MyTableModel();
    QString strSelect;

    if(delete_last_symbol &&
            (filters->last_applied_filter() == ID_FILTER || filters->last_applied_filter() == BEGIN_TEXT_FILTER ||
             filters->last_applied_filter() == PARTS_TEXT_FILTER)){
        input->delete_last_symbol();
        return;
    }

    // сбрасываем текущее значение single_group
    this->single_group = -1;
    this->single_group_without_limits = -1;

    // если надо перезаполнить боксы (т.е. если фильтр подразумевает изменение набора строк),
    // сбрасываем флаг boxes_filled и очищаем заголовки кнопок
    if(filters->last_applied_filter() != SORT_FILTER && filters->last_applied_filter() != LIMITS_FILTER
            && filters->last_applied_filter() != PRICES_FILTER && filters->last_applied_filter() != QUANTITY_FILTER){
        boxes_filled = false;
        clear_button_labels();
    }

    bool reset_page = false;
    if(filters->last_applied_filter() != LIMITS_FILTER){
        pair p = {0, white_table->get_items_on_page()};
        filters->set_limits(p);
        reset_page = true;
    }

    if(filters->last_applied_filter() != NOLIMITS_FILTER && filters->last_applied_filter() != LIMITS_FILTER){
        pair limits = {0, this->white_table->get_items_on_page()};
        //qDebug() << this->white_table->get_items_on_page();
        filters->set_limits(limits);
        filters->restore_limits();
        white_table->set_multipage_mode();
    }
    strSelect = apply_filters(filters);
    //qDebug() << strSelect;

    if(strSelect == "FAIL"){
        // если последний фильтр - текстовый, удаляем последний символ из текстового поля, таблица после этого перезаполнится
        if(filters->last_applied_filter() == ID_FILTER || filters->last_applied_filter() == BEGIN_TEXT_FILTER ||
                filters->last_applied_filter() == PARTS_TEXT_FILTER)
            input->delete_last_symbol();
    }
    else{
        // фильтры применены успешно. Начинаем заполнять поисковик данными.
        // применяем sql-запрос
        query->setQuery(strSelect);
        // сохраняем оригинальные имена столбцов, чтобы узнавать потом индексы
        original_column_names.clear();
        for(int i = 0; i < query->columnCount(); i++)
            original_column_names << query->headerData(i, Qt::Horizontal).toString();

        // переименовываем столбцы
        // если single_group > 0, т.е. выбрана конкретная подгруппа,
        // столбцы с параметрами надо переименовать параметрами для этой подгруппы.
        // для этого выбираем их названия из БД и запихиваем в params_names
        if(single_group > 0)
            params_names = get_params_names(single_group);
        else params_names.clear();
        // и теперь для каждого столбца устанавливаем новое название
        for(int i = 0; i < query->columnCount(); i++)
            query->setHeaderData(i, Qt::Horizontal, rename_column(query->headerData(i, Qt::Horizontal).toString(), single_group));

        // установим названия кнопок над селекторами, только текст, без подсчёта
        set_text_button_labels();

        // если установлен фильтр по цене, надо соответствующим образом раскрасить заголовок
        if(filters->prices_filter() == POSITIVE_PRICES)
            query->setHeaderData(original_column_names.indexOf("price_ret"), Qt::Horizontal, QColor(0,192,0), Qt::BackgroundRole);
        else if(filters->prices_filter() == ZERO_PRICES)
            query->setHeaderData(original_column_names.indexOf("price_ret"), Qt::Horizontal, Qt::red, Qt::BackgroundRole);

        // если установлен фильтр по кол-ву, тоже красим заголовок
        if(filters->quantity_filter() == POSITIVE_QUANTITIES)
            query->setHeaderData(original_column_names.indexOf("quantity"), Qt::Horizontal, QColor(0,192,0), Qt::BackgroundRole);


        // устанавливаем названия кнопок над селекторами
        //set_button_labels();

        // сохраняем столбец и порядок сортировки
        int sort_column;
        Qt::SortOrder sort_order;
        if(!(filters->white_sort_order_filter().isEmpty())){
            SortingOrder s = filters->white_sort_order_filter().first();
            sort_column = original_column_names.indexOf(s.column);
            sort_order = s.order;
        }
        else {
            sort_column = original_column_names.indexOf(DEFAULT_WHITE_SORT_COLUMN);
            sort_order = DEFAULT_WHITE_SORT_ORDER;
            error("Внимание!", "Установлен порядок сортировки по умолчанию");
        }
        white_table->fill(query, original_column_names, sort_column, sort_order, reset_page);
    }
}

QString WhiteSearcher::apply_filters(Filters *filters){
    QSqlDatabase::database().transaction();
    QString query, tables, where, groupby, order, limits = "", count, final;
    int group = filters->group_filter();
    QStringList columns = filters->columns_filter();
    int begin = filters->begin();
    int limit = this->white_table->get_items_on_page();
    int id = filters->white_id_filter();
    short mode = filters->white_id_mode_filter();
    QString beginname = filters->beginname_filter();
    QStringList parts = filters->parts_filter();
    QStringList params = filters->params_filter();
    int prices = filters->prices_filter();
    int quantities = filters->quantity_filter();
    int total;
    bool condition = false;                 // определяет, начали ли мы формировать условие WHERE, т.е. нужно ли вставлять AND

    // список таблиц
    tables = " FROM " + SUBGROUPS_TABLE + " s JOIN " + TOVMARKS_TABLE + " t ON t.subgroup_id = s.id ";

    // первооснова для условия
    where = "";

    // применим фильтр по группам
    if(group == 0)
        where = "";
    else if(group < ENLARGER){
        where = " WHERE t.subgroup_id=" + QString::number(group);
        condition = true;
    }
    else{
        where = " WHERE t.subgroup_id IN (SELECT id FROM " + SUBGROUPS_TABLE + " WHERE group_id=" + QString::number(group-ENLARGER) + ")";
        condition = true;
    }

    if(id != 0){
        // применяем фильтр по id (белому)
        // если условие уже имело место, добавляем AND. Если нет, начинаем со слова WHERE
        where += (condition ? " AND" : " WHERE");
        where += " t.id";
        // ищем ли  мы только равные айдишники, или же не меньшие
        where += (mode == EQUAL ? " =" : " >=");
        where += QString::number(id);
        condition = true;
    }

    if(beginname != ""){
        // применяем фильтр по имени (с начала)
        // если условие уже имело место, добавляем AND. Если нет, начинаем со слова WHERE
        where += (condition ? " AND" : " WHERE");
        where += " t.pattern LIKE '" + beginname + "%'";
        condition = true;
    }

    if(!parts.empty()){
        // применяем фильтр по фрагментам
        where += (condition ? " AND (" : " WHERE (");
        int size = parts.size();
        for(int i = 0; i < size-1; i++){
            where += (" t.pattern LIKE '" + parts.at(i) + "' OR");
        }
        where += " t.pattern LIKE '" + parts.at(size-1) + "')";
        condition = true;
    }

    // применим фильтр по параметрам
    for(int i = 0; i < MAX_PARAMS; i++){
        if(params.at(i).length() != 0){
            where += (condition ? " AND (" : " WHERE (");
            where += " par" + QString::number(i+1) + "_val = '" + params.at(i) + "')";
            condition = true;
        }
    }

    // применим фильтр по ценам
    if(prices == POSITIVE_PRICES){
        where += (condition ? " AND (" : " WHERE (");
        where += " t.price_ret > 0)";
        condition = true;
    }
    else if(prices == ZERO_PRICES){
        where += (condition ? " AND (" : " WHERE (");
        where += " t.price_ret = 0)";
        condition = true;
    }

    // фильтр по количеству
    if(quantities == POSITIVE_QUANTITIES){
        where += (condition ? " AND (" : " WHERE (");
        where += " t.quantity > 0) ";
        condition = true;
    }

    // Для переключателя страниц необходимо подсчитать общее количество записей, удовлетворяющих фильтрам (кроме limits).
    // В table же мы передаём передаём только записи по количеству limits. Поэтому сформируем запрос для подсчёта общего кол-ва записей,
    // выполним его и забудем про это дело.
    count = "SELECT count(*) FROM " + TOVMARKS_TABLE + " t " + where;
    total = set_totals(count);
    if(total <= 0){
        return "FAIL";
    }
    else{
        white_table->set_totals(total);
    }

    // порядок сортировки
    order = " ORDER BY " + sorting_order_to_string(filters->white_sort_order_filter());

    // лимиты
    if(filters->are_there_limits()){
        limits = " OFFSET " + QString::number(begin) + " LIMIT " + QString::number(limit);
    }
    //qDebug("params...");
    // сформируем списки параметров деталей
    //get_params_list(last_applied_filter, tables, where);
    tables_string = tables;
    where_string = where;

    // если все детали, удовлетворяющие запросу, принадлежат только одной подгруппе, её номер надо установить в single_group
    // то же, но без учёта лимитов - в single_group_without_limits. Потом объяснить, что к чему!
    // а вообще это не дело, впилить потом в куда-то
    //qDebug("done.");
    QSqlQuery select_groups;
    query = "SELECT DISTINCT t.subgroup_id FROM (SELECT " + ALL_WHITE_COLUMNS.join(",") + tables + where + ") AS t";
    if(!select_groups.exec(query)){
        error("Ошибка при выборе групп 1", "Не удалось выполнить запрос:\n" + query);
    }
    select_groups.next();
    single_group_without_limits = (select_groups.size() == 1 ? select_groups.value(0).toInt() : -1);
    if(single_group_without_limits != -1)
        single_group = single_group_without_limits;
    else{
        query = "SELECT DISTINCT t.subgroup_id FROM (SELECT " + ALL_WHITE_COLUMNS.join(",") + tables + where + limits + ") AS t";
        if(!select_groups.exec(query)){
            error("Ошибка при выборе групп 2", "Не удалось выполнить запрос:\n" + query);
        }
        select_groups.next();
        single_group = (select_groups.size() == 1 ? select_groups.value(0).toInt() : -1);
    }

    // теперь сформируем строку, которую надо вернуть функции заполнения таблицы
    final = "SELECT " + columns.join(",") + tables + where + groupby + order + limits;
    while(!QSqlDatabase::database().commit());
    return final;
}


