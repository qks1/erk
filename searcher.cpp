#include "searcher.h"
#include "constants.h"
#include <QtGui>
#include <QtSql>

inline void Searcher::initialize(){

}

Searcher::Searcher(QWidget *parent) :
    QWidget(parent)
{
    // инициализируем переменные
    initialize();

    // В конструкторе класса создаём необходимые объекты
    catalog = new Catalog();
    white_table = new Table();

    filters.set_begin(0);

    // заполняем таблицу, пока без фильтров
    fillTable();

    // размещаем все элементы
    layout();

    // наконец, соединяем сигналы со слотами
    connects();

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
    splt->addWidget(this->white_table);

    // устанавливаем соотношение размеров 1:4
    QList<int> sz;
    sz << 100 << 400;
    splt->setSizes(sz);

    return splt;
}


inline void Searcher::connects(){
    // соединяем сигналы со слотами

    // при получении сигнала о смене группы - устанавливать фильтр
    QObject::connect(this->catalog, SIGNAL(group_changed(int)),
                     this, SLOT(set_group_filter(int)));
}


QStringList Searcher::get_columns_list(){
    QStringList lst;
    lst << "id" << "name" << "subgroup_id" << "price_ret";
    this->filters.set_columns_filter(lst);
    return filters.columns_filter();
}

void Searcher::fillTable(){
    QStringList columns = get_columns_list();
    QSqlQuery query;
    QString strSelect = apply_filters();

    if(!query.exec(strSelect)){
        error("Ошибка", QString("Не удалось выполнить запрос:\n").append(strSelect));
    }

    white_table->fill_table(query, columns);
}

void Searcher::set_group_filter(int group){
    this->filters.set_group_filter(group);
    fillTable();
}

void Searcher::resizeEvent(QResizeEvent *){

}

QString Searcher::apply_filters(){
    QString query = "";
    int group = this->filters.group_filter();
    QStringList columns = this->filters.columns_filter();
    int begin = this->filters.begin_value();
    int limit = this->white_table->items_on_page;

    if(group == 0)
        query = "SELECT " + columns.join(",") + " FROM " + TOVMARKS_TABLE + " ORDER BY id OFFSET " + QString::number(begin) + " LIMIT " + QString::number(limit);
    else if(group < ENLARGER)
        query = "SELECT " + columns.join(",") + " FROM " + TOVMARKS_TABLE + " WHERE subgroup_id=" + QString::number(group) + " ORDER BY id OFFSET " + QString::number(begin) + " LIMIT " + QString::number(limit);
    else
        query = "SELECT " + columns.join(",") + " FROM " + TOVMARKS_TABLE + " WHERE subgroup_id IN (SELECT id FROM " + SUBGROUPS_TABLE + " WHERE group_id=" + QString::number(group-ENLARGER) + ") ORDER BY id OFFSET " + QString::number(begin) + " LIMIT " + QString::number(limit);
    return query;
}
