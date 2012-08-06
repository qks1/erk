#include "searcher.h"

Searcher::Searcher(QWidget *parent) :
    QAbstractScrollArea(parent)
{
    // В конструкторе класса создаём необходимые объекты
    catalog = new Catalog();
    if (!catalog->ok){
        this->ok = false;
        return;
    }
    white_table = new Table(this);
    grey_table = new Table(this);
    active_table = white_table;
    input = new Input();
    ParamsSelector *param = 0;
    for(int i = 0; i < MAX_PARAMS; i++){
        //qDebug() << "(searcher, constructor)" << QString::number(i);
        param = new ParamsSelector(i);
        selectors << param;
    }
    params_reset = new QPushButton("X");
    max_params = 0;
    filters = new Filters();
    pair limits = {0, this->white_table->get_items_on_page()};
    filters->set_limits(limits);
    filters->reset_white_id();
    filters->reset_beginname();
    filters->set_group_filter(0);
    filters->set_columns_filter(WHITE_TABLE_COLUMNS);
    filters->set_prices(ALL_PRICES);
    SortingOrder order = {DEFAULT_WHITE_SORT_COLUMN, DEFAULT_WHITE_SORT_ORDER};
    filters->add_white_sort_column(order);
    filters->reset_params();
    last_applied_filter = GROUPS_FILTER;
    single_group = -1;
    single_group_without_limits = -1;

    // сопоставляем имена столбцов в таблице именам столбцов в базе
    fill_column_names();

    // заполняем таблицу, пока без фильтров
    fill_table(false, false);

    // размещаем все элементы
    set_layout();

    // наконец, соединяем сигналы со слотами
    connects();
    ok = true;
}


inline void Searcher::set_layout(){
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
    selectors_panel->setMinimumWidth(50*MAX_PARAMS);
    selectors_panel->setFixedHeight(selectors.at(0)->height());

    // формируем разделитель, содержащий каталог и таблицу
    QSplitter *splitter = split();

    // главный layout
    QVBoxLayout *mainlt= new QVBoxLayout;
    mainlt->addWidget(top_panel);
    mainlt->addWidget(selectors_panel);
    mainlt->addWidget(splitter);

    grey_table->hide();

    this->setLayout(mainlt);
}

QSplitter *Searcher::split(){
    // формируем разделитель, содержащий каталог и таблицу
    QSplitter* splt = new QSplitter(Qt::Horizontal);
    splt->addWidget(this->catalog);
    splt->addWidget(this->active_table);

    // устанавливаем соотношение размеров 1:4
    QList<int> sz;
    sz << 110 << 400;
    splt->setSizes(sz);

    return splt;
}

inline void Searcher::fill_column_names(){
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
}

inline void Searcher::connects(){
    // соединяем сигналы со слотами

    // при получении сигнала о смене группы - устанавливать фильтр
    QObject::connect(this->catalog, SIGNAL(group_changed(int)),
                     this, SLOT(set_group_filter(int)));

    // при получении сигнала о смене пределов - устанавливать фильтр
    QObject::connect(this->active_table, SIGNAL(limits_changed(pair)),
                     this, SLOT(set_limits_filter(pair)));

    // при получении сигнала о снятии лимита записей - установить фильтр nolimits
    QObject::connect(this->active_table, SIGNAL(limits_removed()),
                     this, SLOT(set_nolimits()));

    // при получении сигнала об установке лимита - снять фильтр nolimits
    QObject::connect(this->active_table, SIGNAL(limits_restored()),
                     this, SLOT(set_limits()));

    // при получении сигнала об изменении текста в текстовом поле - устанавливаем фильтр
    QObject::connect(this->input, SIGNAL(text_changed_signal(int, QString)),
                     this, SLOT(set_text_filter(int, QString)));

    // при получении сигнала об очищении текстового поля - очищать текстовые фильтры
    QObject::connect(this->input, SIGNAL(reset_signal()),
                     this, SLOT(reset_text_filters()));

    // при получении сигнала об изменении столбца и порядка сортировки - менять соответствующие фильтры
    QObject::connect(this->active_table, SIGNAL(sort_order_changed(int, Qt::SortOrder)),
                     this, SLOT(change_sort_order(int, Qt::SortOrder)));

    for(int i = 0; i < selectors.size(); i++){
        // при получении сигнала о выборе элемента из селектора - устанавливать params_filter
        QObject::connect(this->selectors.at(i), SIGNAL(item_selected(QString, int)),
                         this, SLOT(set_param_filter(QString, int)));

        // при получении сигнала о нажатии на кнопку - производить сортировку
        QObject::connect(this->selectors.at(i), SIGNAL(button_clicked(int)),
                         this, SLOT(button_sort(int)));
    }

    QObject::connect(this->params_reset, SIGNAL(clicked()),
                     this, SLOT(reset_params_filter()));

    QObject::connect(this->active_table, SIGNAL(prices_clicked()),
                     this, SLOT(switch_prices_filter()));



}

QString Searcher::rename_column(QString str, int group){
    QRegExp rx("^(par[0-9]+)_val$");
    if(group > 0 && str.contains(rx)){
        //
        int pos = rx.indexIn(str);
        return (pos > -1 ? params_names[rx.cap(1)] : "?");
    }
    else
        return column_names[str];
}

QMap<QString, QString> Searcher::get_params_names(int group){
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

//-----------------------------------------------------------------//

void Searcher::fill_boxes(){
    int i;
    if(last_applied_filter != SORT_FILTER && last_applied_filter != LIMITS_FILTER
            && last_applied_filter != PRICES_FILTER){
        max_params = 0;
        // надо найти ПОСЛЕДНИЙ список ненулевого размера
        for(i = MAX_PARAMS-1; i >= 0; i--){
            if(params_lists.at(i).count() > 0){
                max_params = i;
                break;
            }
        }
        for(i = 0; i <= max_params; i++){
            selectors.at(i)->clear_items();
            // в начале списка добавляем пустой айтем
            selectors.at(i)->add_item("");
            selectors.at(i)->add_items(params_lists.at(i));
            if(params_lists.at(i).count() == 1)
                selectors.at(i)->set_selected(1);
            selectors.at(i)->setEnabled(params_lists.at(i).count() == 0 ? false : true);
        }
        for(; i < MAX_PARAMS; i++)
            selectors.at(i)->setEnabled(false);
    }
}

void Searcher::set_button_labels(){
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

//-----------------------------------------------------------------//

void Searcher::fill_table(bool reset_groups = false, bool delete_last_symbol = false){
    MyTableModel *query = new MyTableModel();
    QString strSelect;

    if(delete_last_symbol &&
            (last_applied_filter == ID_FILTER || last_applied_filter == BEGIN_TEXT_FILTER || last_applied_filter == PARTS_TEXT_FILTER)){
        input->delete_last_symbol();
        return;
    }

    // сбрасываем текущее значение single_group
    this->single_group = -1;
    this->single_group_without_limits = -1;

    if(reset_groups){
        pair limits = this->filters->limits_filter();
        limits.end -= limits.begin;
        limits.begin = 0;
        this->filters->set_limits(limits);
        last_applied_filter = SETLIMITS_FILTER;
    }

    if(last_applied_filter != NOLIMITS_FILTER && last_applied_filter != LIMITS_FILTER){
        pair limits = {0, this->active_table->get_items_on_page()};
        //qDebug() << this->active_table->get_items_on_page();
        filters->set_limits(limits);
        filters->restore_limits();
        active_table->set_multipage_mode();
    }
    strSelect = apply_filters();
    //qDebug() << strSelect;


    if(strSelect == "FAIL"){
        // если последний фильтр - текстовый, удаляем последний символ из текстового поля, таблица после этого перезаполнится
        if(last_applied_filter == ID_FILTER || last_applied_filter == BEGIN_TEXT_FILTER || last_applied_filter == PARTS_TEXT_FILTER){
            input->delete_last_symbol();
        }
    }
    else{
        // фильтры применены успешно. Начинаем заполнять поисковик данными.
        // заполняем комбобоксы с параметрами
        //fill_boxes();
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

        // если установлен фильтр по цене, надо соответствующим образом раскрасить заголовок
        if(this->filters->prices_filter() == POSITIVE_PRICES)
            query->setHeaderData(original_column_names.indexOf("price_ret"), Qt::Horizontal, QColor(0,192,0), Qt::BackgroundRole);
        else if(this->filters->prices_filter() == ZERO_PRICES)
            query->setHeaderData(original_column_names.indexOf("price_ret"), Qt::Horizontal, Qt::red, Qt::BackgroundRole);


        // устанавливаем названия кнопок над селекторами
        set_button_labels();

        // сохраняем столбец и порядок сортировки
        int sort_column;
        Qt::SortOrder sort_order;
        if(!(this->filters->white_sort_order_filter().isEmpty())){
            SortingOrder s = this->filters->white_sort_order_filter().first();
            sort_column = original_column_names.indexOf(s.column);
            sort_order = s.order;
        }
        else {
            sort_column = original_column_names.indexOf(DEFAULT_WHITE_SORT_COLUMN);
            sort_order = DEFAULT_WHITE_SORT_ORDER;
            error("Внимание!", "Установлен порядок сортировки по умолчанию");
        }
        active_table->fill(query, original_column_names, sort_column, sort_order, this->filters->prices_filter(), single_group, reset_groups);
    }
}

int Searcher::set_totals(QString query){
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

void Searcher::reset_text_filters(){
    this->filters->reset_params();
    this->filters->reset_white_id();
    this->filters->reset_beginname();
    this->input->reset_text();
}

void Searcher::set_group_filter(int group){
    this->filters->reset_params();
    this->reset_text_filters();
    this->filters->set_group_filter(group);
    last_applied_filter = GROUPS_FILTER;
    fill_table(true, false);
}

void Searcher::reset_group_filter(){
    this->filters->reset_params();
    this->filters->set_group_filter(0);
    last_applied_filter = GROUPS_FILTER;
    fill_table(true, false);
}

void Searcher::set_limits_filter(pair limits){
    this->filters->set_limits(limits);
    last_applied_filter = LIMITS_FILTER;
    fill_table();
}

void Searcher::set_nolimits(){
    this->filters->remove_limits();
    last_applied_filter = NOLIMITS_FILTER;
    fill_table();
}

void Searcher::set_limits(){
    this->filters->restore_limits();
    last_applied_filter = SETLIMITS_FILTER;
    fill_table();
}

void Searcher::set_text_filter(int type, QString text){
    // сбрасываем фильтр по группам
    this->filters->set_group_filter(0);
    // и по параметрам
    this->filters->reset_params();
    // и текущее значение текстового фильтра - тоже, если ранее текстовое поле было очищено
    //this->reset_text_filters();

    if(type == ID_MODE){
        // устанавливаем фильтр по id
        this->filters->set_white_id(text.toInt());
        if(this->input->id_combobox_value() == 0)
            this->filters->set_white_id_mode(GREATER_OR_EQUAL);
        else if(this->input->id_combobox_value() == 1)
            this->filters->set_white_id_mode(EQUAL);
        last_applied_filter = ID_FILTER;
    }
    else if(type == BEGIN_MODE){
        // поиск с начала
        text = replases(text);
        this->filters->set_beginname(text.toUpper());
        last_applied_filter = BEGIN_TEXT_FILTER;
    }
    else if(type == PARTS_MODE){
        // поиск по фрагментам
        text = replases(text);
        // текст в текстовом поле надо преобразовать в список фрагментов
        QStringList parts = text.split(" ", QString::SkipEmptyParts);
        QStringList patterns;
        QString tmpstring;
        if(parts.size() != 0){
            patterns = valid_strings(parts);
            // а если ни одного валидного паттерна не нашлось, то сразу вызываем fill_table, чтобы она удалила последний символ
            if(patterns.size() == 0){
                fill_table(false, true);
                return;
            }
            //foreach (tmpstring, patterns)  qDebug() << tmpstring;
        }
        else{
            patterns.clear();
        }
        this->filters->set_parts(patterns);
        last_applied_filter = PARTS_TEXT_FILTER;
    }
    else{
        // какая-то хрень, ничего не делаем
    }
    fill_table();
}

void Searcher::change_sort_order(int column, Qt::SortOrder order){
    QString column_name = original_column_names[column];
    SortingOrder s = {column_name, order};
    this->filters->add_white_sort_column(s);
    last_applied_filter = SORT_FILTER;
    fill_table();
}

void Searcher::button_sort(int num){
    int column_index = original_column_names.indexOf("par" + QString::number(num+1) + "_val");
    int current_sort_index = original_column_names.indexOf(this->filters->white_sort_order_filter().first().column);
    Qt::SortOrder order;
    if(column_index == current_sort_index)
        order = (this->filters->white_sort_order_filter().first().order == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
    else
        order = Qt::AscendingOrder;
    change_sort_order(column_index, order);
}

void Searcher::set_param_filter(QString param, int num){
    this->filters->set_param(num, param);
    last_applied_filter = PARAMS_FILTER;
    fill_table();
}

void Searcher::reset_params_filter(){
    this->filters->reset_params();
    last_applied_filter = PARAMS_FILTER;
    fill_table();
}

void Searcher::switch_prices_filter(){
    this->filters->switch_prices();
    last_applied_filter = PRICES_FILTER;
    fill_table();
}

QStringList Searcher::valid_strings(QStringList parts){
    // фрагменты будем обрабатывать по очереди, т.е. сначала рассмотрим последовательность из 1 фрагмента,
    // затем из 2, 3 и т.д.
    // На каждом этапе будем отсекать те последовательности, по которым не находится ни одной позиции,
    // а оставшиеся будем хранить в двумерном списке valid.
    QList<QStringList> valid;
    // также нам понадобится временный двумерный список,
    //куда мы будем складывать валидные варианты на каждом этапе.
    QList<QStringList> temp_valid;
    // первоначально в списке valid будет храниться только первый фрагмент.
    // Если и по нему ничего не находится - прикрываем лавочку.
    QStringList templist;
    int result_size;
    templist.insert(0, parts.at(0));
    result_size = size_of_select(templist);
    //qDebug() << QString::number(result_size);
    // если поиск по первому фрагменту не вернул нифига, значит, поиск по введённым фрагментам не должен найти ничего.
    // Поэтому вернём лист, состоящий из единственного этого первого фрагмента, чтобы потом установить его в parts_filter,
    // тогда apply_filters сформирует строку запроса, возвращающую ноль строк, то есть FAIL
    if(result_size == 0){
        return templist;
    }
    // если вернул, то продолжаем извращения
    else{
        valid.insert(0, templist);
        // для каждого фрагмента из списка:
        for(int i = 1; i < parts.size(); i++){
            temp_valid.clear();
            // берём каждый валидный stringlist
            for(int j = 0; j < valid.size(); j++){
                //между каждыми двумя строчками из этого листа, а также в начале и в конце
                for(int k = 0; k <= valid.at(j).size(); k++){
                    // if(valid.at(j).indexOf(parts.at(i)) != -1) continue;
                    // по очереди вставляем строку из parts
                    templist = valid.at(j);
                    templist.insert(k, parts.at(i));
                    // и проверяем получившуюся строку на валидность.
                    result_size = size_of_select(templist);
                    // если валидна, вставляем её в temp_valid
                    if(result_size > 0){
                        temp_valid << templist;
                    }
                }
            }
            valid.clear();
            // если на данном этапе не нашлось ни одной валидной строки, то кранты - надо возвращать пустой лист
            if(temp_valid.size() == 0){
                break;
            }
            valid = temp_valid;
        }

        // это уже под конец
        // в valid лежат последовательности фрагментов, каждую из которых нужно склеить в строку для запроса
        QStringList patterns;
        foreach (templist, valid) {
            patterns << "%" + templist.join("%") + "%";
        }
        return patterns;
    }
}


int Searcher::size_of_select(QString str){
    QSqlQuery query;
    //qDebug() << query_string;
    query.exec(str);
    return query.size();
}

int Searcher::size_of_select(QStringList lst){
    QString str = "SELECT * FROM " + TOVMARKS_TABLE + " WHERE pattern LIKE '%" + lst.join("%") + "%'";
    return size_of_select(str);
}

QString Searcher::sorting_order_to_string(QList<SortingOrder> order){
    QStringList str;
    QStringList s_columns;
    s_columns << "subgroup";
    foreach(SortingOrder s, order){
        // пока костыль
        str << (s_columns.indexOf(s.column) >= 0 ? "" : "t.") + s.column + (s.order == Qt::AscendingOrder ? " ASC" : " DESC");
    }
    return str.join(", ");
}



void Searcher::resizeEvent(QResizeEvent *){
    //input->setGeometry(this->width()-320, 0, 320, 70);
}

QStringList Searcher::sql_to_list(QSqlQuery query){
    QStringList list;
    // поскольку в переданном QSqlQuery мы выбирали только один столбец,
    // искомая строка находится в позиции 0
    while (query.next()){
        list << query.value(0).toString();
    }
    return list;
}

void Searcher::get_params_list(QString tables, QString where){
    QString query_str, par_name;
    QStringList list;
    QSqlQueryModel* query = 0;
    int count;
    // сформируем списки параметров деталей
    // для этого в классе есть лист двумерный QStringList, куда и будем их складывать
    if(last_applied_filter != SORT_FILTER && last_applied_filter != LIMITS_FILTER
            && last_applied_filter != PRICES_FILTER){
        for(int i = 0; i < MAX_PARAMS; i++){
            query = new QSqlQueryModel();
            par_name = "t.par" + QString::number(i+1) + "_val";
            list.clear();
            selectors.at(i)->clear_items();
            query_str = "(SELECT '') UNION (SELECT " + par_name + tables + where + " AND " + par_name + "!= '')  ORDER BY 1";
            query->setQuery(query_str);
            qDebug() << query_str;
            count = query->rowCount();
            qDebug() << QString::number(count);
            if(count == 1){
                // если список пуст, делаем комбобокс неактивным
                selectors.at(i)->setEnabled(false);
            }
            else{
                //qDebug() << QString::number(count);
                selectors.at(i)->items->setModel(query);
                selectors.at(i)->items->setModelColumn(0);
                if(count == 2)
                    selectors.at(i)->set_selected(1);
                selectors.at(i)->setEnabled(true);
            }
        }
    }
    delete query;
}

QString Searcher::apply_filters(){
    QSqlDatabase::database().transaction();
    QString query, tables, where, order, limits = "", count, final;
    int group = this->filters->group_filter();
    QStringList columns = this->filters->columns_filter();
    int begin = this->filters->begin();
    int limit = this->white_table->get_items_on_page();
    int id = this->filters->white_id_filter();
    short mode = this->filters->white_id_mode_filter();
    QString beginname = this->filters->beginname_filter();
    QStringList parts = this->filters->parts_filter();
    QStringList params = this->filters->params_filter();
    int prices = this->filters->prices_filter();
    int total;
    bool condition = false;                 // определяет, начали ли мы формировать условие WHERE, т.е. нужно ли вставлять AND

    // список таблиц
    tables = " FROM " + TOVMARKS_TABLE + " t, " + SUBGROUPS_TABLE + " s";


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
    // Для переключателя страниц необходимо подсчитать общее количество записей, удовлетворяющих фильтрам (кроме limits).
    // В table же мы передаём передаём только записи по количеству limits. Поэтому сформируем запрос для подсчёта общего кол-ва записей,
    // выполним его и забудем про это дело.
    count = "SELECT count(*) FROM " + TOVMARKS_TABLE + " t " + where;
    total = set_totals(count);
    if(total <= 0){
        return "FAIL";
    }
    else{
        active_table->set_totals(total);
    }

    // добавляем теперь условие для получения названий подгрупп
    where += (condition ? " AND" : " WHERE");
    where += " s.id = t.subgroup_id";

    // порядок сортировки
    order = " ORDER BY " + sorting_order_to_string(this->filters->white_sort_order_filter());

    // лимиты
    if(this->filters->are_there_limits()){
        limits = " OFFSET " + QString::number(begin) + " LIMIT " + QString::number(limit);
    }
    qDebug("params...");
    // сформируем списки параметров деталей
    get_params_list(tables, where);

    // если все детали, удовлетворяющие запросу, принадлежат только одной подгруппе, её номер надо установить в single_group
    // то же, но без учёта лимитов - в single_group_without_limits. Потом объяснить, что к чему!
    // а вообще это не дело, впилить потом в куда-то
    qDebug("groups1...");
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
        qDebug("groups2...");
        query = "SELECT DISTINCT t.subgroup_id FROM (SELECT " + ALL_WHITE_COLUMNS.join(",") + tables + where + limits + ") AS t";
        if(!select_groups.exec(query)){
            error("Ошибка при выборе групп 2", "Не удалось выполнить запрос:\n" + query);
        }
        select_groups.next();
        single_group = (select_groups.size() == 1 ? select_groups.value(0).toInt() : -1);
    }
    // теперь сформируем строку, которую надо вернуть функции заполнения таблицы
    final = "SELECT " + columns.join(",") + tables + where + order + limits;
    qDebug("ok");
    while(!QSqlDatabase::database().commit());
    return final;
}
