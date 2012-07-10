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
    filters = new Filters();
    pair limits = {0, this->white_table->get_items_on_page()};
    filters->set_limits(limits);

    // заполняем таблицу, пока без фильтров
    fill_table(false);

    // размещаем все элементы
    layout();

    // наконец, соединяем сигналы со слотами
    connects();
    ok = true;
}


inline void Searcher::layout(){
    // формируем разделитель, содержащий каталог и таблицу
    splitter = split();

    // собственно, размещение
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(splitter);
    this->setLayout(layout);
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

}


QStringList Searcher::get_columns_list(){
    QStringList lst;
    lst << "id" << "name" << "subgroup_id" << "price_ret" << "par1_val" << "par2_val" << "par3_val" << "par4_val";
    this->filters->set_columns_filter(lst);
    return filters->columns_filter();
}

void Searcher::fill_table(bool reset_groups){
    QStringList columns = get_columns_list();       // пока так, нуачо
    QSqlQuery query;
    QString strSelect;

    if(reset_groups){
        pair limits = this->filters->limits_filter();
        limits.end -= limits.begin;
        limits.begin = 0;
        this->filters->set_limits(limits);
    }
    strSelect = apply_filters();

    if(!query.exec(strSelect)){
        error("Ошибка", QString("Не удалось выполнить запрос:\n").append(strSelect));
    }

    active_table->fill(query,columns,reset_groups);
}

void Searcher::set_totals(QString query){
    QSqlQuery sql;
    int total;
    if(!sql.exec(query)){
        error("Ошибка", QString("Не удалось выполнить запрос:\n").append(query));
    } else {
        sql.next();
        total = sql.value(0).toInt();
        active_table->set_totals(total);
    }

}

void Searcher::set_group_filter(int group){
    this->filters->set_group_filter(group);
    fill_table(true);
}

void Searcher::set_limits_filter(pair limits){
    this->filters->set_limits(limits);
    fill_table(false);
}

void Searcher::set_nolimits(){
    this->filters->remove_limits();
    fill_table(false);
}

void Searcher::set_limits(){
    this->filters->restore_limits();
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

    // первооснова для запроса
    base = "SELECT " + columns.join(",") + " FROM " + TOVMARKS_TABLE;

    // применим фильтр по группам
    if(group == 0)
        where = "";
    else if(group < ENLARGER)
        where = " WHERE subgroup_id=" + QString::number(group);
    else
        where = " WHERE subgroup_id IN (SELECT id FROM " + SUBGROUPS_TABLE + " WHERE group_id=" + QString::number(group-ENLARGER) + ")";

    // сформируем строку запроса для получения кол-ва записей
    count = "SELECT count(*) FROM " + TOVMARKS_TABLE + where;
    // найдём кол-во записей и установим его в table
    set_totals(count);

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
