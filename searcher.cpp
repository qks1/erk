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
    int max_params = 0;
    filters = new Filters();
    pair limits = {0, this->white_table->get_items_on_page()};
    filters->set_limits(limits);
    filters->reset_white_id();
    filters->reset_beginname();
    filters->set_group_filter(0);
    SortingOrder order = {DEFAULT_WHITE_SORT_COLUMN, DEFAULT_WHITE_SORT_ORDER};
    filters->add_white_sort_column(order);
    filters->reset_params();
    last_applied_filter = GROUPS_FILTER;


    // заполняем таблицу, пока без фильтров
    fill_table(false, false);

    // размещаем все элементы
    set_layout();

    // наконец, соединяем сигналы со слотами
    connects();
    ok = true;
}


inline void Searcher::set_layout(){
    // горизонтальный layout, содержащий выпадающие списки параметров и текстовое поле
    QHBoxLayout *hlt = new QHBoxLayout;
    hlt->setSpacing(0);
    hlt->setMargin(0);
    for(int i = 0; i < MAX_PARAMS; i++){
        hlt->addWidget(selectors.at(i));
        selectors.at(i)->setMinimumWidth(60);
        //QRect rect(0,0,50,20);
        //selectors.at(i)->setGeometry(rect);

    }
    hlt->addWidget(input);
    //hlt->addStretch(2);
    //input->setGeometry(this->width()-320, 0, 320, 70);
    input->setFixedSize(320, 70);
    //hlt->setSizeConstraint(QLayout::SetMaximumSize);

    // формируем разделитель, содержащий каталог и таблицу
    splitter = split();

    // собственно, размещение
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(splitter);

    // главный layout
    QVBoxLayout *mainlt= new QVBoxLayout;
    mainlt->addLayout(hlt);
    mainlt->addLayout(layout);
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
    QObject::connect(this->active_table, SIGNAL(sort_order_changed(QString, Qt::SortOrder)),
                     this, SLOT(change_sort_order(QString, Qt::SortOrder)));

    // при получении сигнала о выборе элемента из селектора - устанавливать params_filter
    for(int i = 0; i < selectors.size(); i++){
        QObject::connect(this->selectors.at(i), SIGNAL(item_selected(QString, int)),
                         this, SLOT(set_param_filter(QString, int)));
    }

}


QStringList Searcher::get_columns_list(){
    QStringList lst;
    lst << "t.id" << "t.name" << "s.name as subgroup" << "t.price_ret" << "t.par1_val" << "t.par2_val" << "t.par3_val" << "t.par4_val"<< "t.par5_val" << "t.par6_val" << "t.par7_val" << "t.par8_val" << "t.par9_val" << "t.par10_val";
    this->filters->set_columns_filter(lst);
    return filters->columns_filter();
}

void Searcher::fill_table(bool reset_groups = false, bool delete_last_symbol = false){
    QStringList columns = get_columns_list();
    MyTableModel *query = new MyTableModel;
    QString strSelect;
    int group_filter = this->filters->group_filter();

    if(delete_last_symbol &&
            (last_applied_filter == ID_FILTER || last_applied_filter == BEGIN_TEXT_FILTER || last_applied_filter == PARTS_TEXT_FILTER)){
            input->delete_last_symbol();
            return;
    }

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
    qDebug() << strSelect;

    if(strSelect == "FAIL"){
        // если последний фильтр - текстовый, удаляем последний символ из текстового поля
        if(last_applied_filter == ID_FILTER || last_applied_filter == BEGIN_TEXT_FILTER || last_applied_filter == PARTS_TEXT_FILTER){
            input->delete_last_symbol();
        }
    }
    else{
        // заполняем комбобоксы с параметрами
        if(last_applied_filter != SORT_FILTER){
            for(int i = 0; i < MAX_PARAMS; i++)
                selectors.at(i)->hide();
            // пока грубо из через жопу, потом разберёмся
            max_params = 0;
            // надо найти ПОСЛЕДНИЙ список ненулевого размера
            for(int i = MAX_PARAMS-1; i >= 0; i--){
                if(params_lists.size() > 0){
                    max_params = i;
                    break;
                }
            }
            for(int i = 0; i <= max_params; i++){
                QString item;
                selectors.at(i)->clear_items();
                // в начале списка добавляем пустой айтем
                selectors.at(i)->add_item("");
                foreach(item, params_lists.at(i)){
                    selectors.at(i)->add_item(item);
                }
                if(params_lists.at(i).size() == 1)
                    selectors.at(i)->set_selected(1);
                selectors.at(i)->show();
            }
        }

        query->setQuery(strSelect);
        QString sort_column;
        Qt::SortOrder sort_order;
        if(!(this->filters->white_sort_order_filter().isEmpty())){
            SortingOrder s = this->filters->white_sort_order_filter().first();
            sort_column = s.column;
            sort_order = s.order;
        }
        else {
            sort_column = DEFAULT_WHITE_SORT_COLUMN;
            sort_order = DEFAULT_WHITE_SORT_ORDER;
            error("Внимание!", "Установлен порядок сортировки по умолчанию");
        }
        if(group_filter <= 0 || group_filter >= ENLARGER)
            group_filter = 0;
        active_table->fill(query, sort_column, sort_order, group_filter, reset_groups);
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

void Searcher::change_sort_order(QString column_name, Qt::SortOrder order){
    SortingOrder s = {column_name, order};
    this->filters->add_white_sort_column(s);
    last_applied_filter = SORT_FILTER;
    fill_table();
}

void Searcher::set_param_filter(QString param, int num){
    this->filters->set_param(num, param);
    last_applied_filter = PARAMS_FILTER;
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

QString Searcher::apply_filters(){
    QString query, tables, where, order, limits = "", count;
    int group = this->filters->group_filter();
    QStringList columns = this->filters->columns_filter();
    int begin = this->filters->begin();
    int limit = this->white_table->get_items_on_page();
    int id = this->filters->white_id_filter();
    short mode = this->filters->white_id_mode_filter();
    QString beginname = this->filters->beginname_filter();
    QStringList parts = this->filters->parts_filter();
    QStringList params = this->filters->params_filter();
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
    //qDebug() << order;

    // лимиты
    if(this->filters->are_there_limits()){
        limits = " OFFSET " + QString::number(begin) + " LIMIT " + QString::number(limit);
    }

    // сформируем списки параметров деталей
    // для этого в классе исть лист двумерный QStringList, куда и будем их складывать
    if(last_applied_filter != SORT_FILTER){
        params_lists.clear();
        for(int i = 1; i <= MAX_PARAMS; i++){
            QSqlQuery select_param;
            query = "SELECT DISTINCT t.par" + QString::number(i) + "_val" + tables + where + " AND t.par" + QString::number(i) + "_val != ''" + " ORDER BY t.par" + QString::number(i) + "_val";
            //qDebug() << query;
            if(!select_param.exec(query)){
                error("Ошибка при выборе параметров", "Не удалось выполнить запрос:\n" + query);
                break;
            }
            else
                params_lists << sql_to_list(select_param);
        }
    }
    // теперь сформируем строку, которую надо вернуть функции заполнения таблицы
    query = "SELECT " + columns.join(",") + tables + where + order + limits;
    return query;
}
