#include "searcher.h"

inline void Searcher::initialize(){

}

Searcher::Searcher(QWidget *parent) :
    QWidget(parent)
{
    // инициализируем переменные
    initialize();
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
    filters = new Filters();
    pair limits = {0, this->white_table->get_items_on_page()};
    filters->set_limits(limits);
    filters->reset_white_id();
    filters->reset_beginname();
    filters->set_group_filter(0);
    last_applied_filter = GROUPS_FILTER;

    // заполняем таблицу, пока без фильтров
    fill_table(false);

    // размещаем все элементы
    set_layout();

    // наконец, соединяем сигналы со слотами
    connects();
    ok = true;
}


inline void Searcher::set_layout(){
    // горизонтальный layout, содержащий выпадающие списки параметров и текстовое поле
    QHBoxLayout *hlt = new QHBoxLayout;
    hlt->setDirection(QBoxLayout::RightToLeft);
    hlt->addWidget(input);
    hlt->addStretch(1);
    input->setFixedSize(320,70);
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
    QObject::connect(this->input, SIGNAL(text_changed_signal(int,QString)),
                     this, SLOT(set_text_filter(int,QString)));

    // при получении сигнала об очищении текстового поля - очищать текстовые фильтры
    QObject::connect(this->input, SIGNAL(reset_signal()),
                     this, SLOT(reset_text_filters()));

}


QStringList Searcher::get_columns_list(){
    QStringList lst;
    lst << "t.id" << "t.name" << "s.name as subgroup" << "t.price_ret" << "t.par1_val" << "t.par2_val" << "t.par3_val" << "t.par4_val"<< "t.par5_val" << "t.par6_val" << "t.par7_val" << "t.par8_val" << "t.par9_val" << "t.par10_val";
    this->filters->set_columns_filter(lst);
    return filters->columns_filter();
}

void Searcher::fill_table(bool reset_groups){
    QStringList columns = get_columns_list();       // пока так, нуачо
    QSqlQueryModel *query = new QSqlQueryModel;
    QString strSelect;

    if(reset_groups){
        pair limits = this->filters->limits_filter();
        limits.end -= limits.begin;
        limits.begin = 0;
        this->filters->set_limits(limits);
    }
    strSelect = apply_filters();
    if(strSelect == "FAIL"){
        // если последний фильтр - текстовый, удаляем последний символ из текстового поля
        if(last_applied_filter == ID_FILTER || last_applied_filter == BEGIN_TEXT_FILTER || last_applied_filter == PARTS_TEXT_FILTER){
            input->delete_last_symbol();
        }
    }
    else{
        query->setQuery(strSelect);
        active_table->fill(query, columns, reset_groups);
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
    this->filters->reset_white_id();
    this->filters->reset_beginname();
    this->input->reset_text();
}

void Searcher::set_group_filter(int group){
    this->reset_text_filters();
    this->filters->set_group_filter(group);
    last_applied_filter = GROUPS_FILTER;
    fill_table(true);
}

void Searcher::reset_group_filter(){
    this->filters->set_group_filter(0);
    last_applied_filter = GROUPS_FILTER;
    fill_table(true);
}

void Searcher::set_limits_filter(pair limits){
    this->filters->set_limits(limits);
    last_applied_filter = LIMITS_FILTER;
    fill_table(false);
}

void Searcher::set_nolimits(){
    this->filters->remove_limits();
    last_applied_filter = LIMITS_FILTER;
    fill_table(false);
}

void Searcher::set_limits(){
    this->filters->restore_limits();
    last_applied_filter = LIMITS_FILTER;
    fill_table(false);
}

void Searcher::set_text_filter(int type, QString text){
    // сбрасываем фильтр по группам
    this->filters->set_group_filter(0);
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
        this->filters->set_beginname(text.toUpper());
        last_applied_filter = BEGIN_TEXT_FILTER;
    }
    else if(type == PARTS_MODE){
        // поиск по фрагментам
    }
    else{
        // какая-то хрень, ничего не делаем
    }
    fill_table(false);
}

void Searcher::resizeEvent(QResizeEvent *){

}

QString Searcher::apply_filters(){
    QString query, base, where, order, limits = "", count;
    int group = this->filters->group_filter();
    QStringList columns = this->filters->columns_filter();
    int begin = this->filters->begin();
    int limit = this->white_table->get_items_on_page();
    int id = this->filters->white_id_filter();
    short mode = this->filters->white_id_mode_filter();
    QString beginname = this->filters->beginname_filter();
    int total;
    bool condition = false;                 // определяет, начали ли мы формировать условие WHERE, т.е. нужно ли вставлять AND

    // первооснова для запроса
    base = "SELECT " + columns.join(",") + " FROM " + TOVMARKS_TABLE + " t, " + SUBGROUPS_TABLE + " s";

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
    order = " ORDER BY id";

    // лимиты
    if(this->filters->are_there_limits()){
        limits = " OFFSET " + QString::number(begin) + " LIMIT " + QString::number(limit);
    }

    // теперь сформируем строку
    query = base + where + order + limits;
    return query;
}
