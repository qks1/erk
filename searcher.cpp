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
    table = new QTableWidget();

    swtch = new switcher();

    // заполняем таблицу, пока без фильтров
    fillTable();  
    // размещаем все элементы
    layout();

    // наконец, соединяем сигналы со слотами
    connects();

}


inline void Searcher::layout(){
    // размещаем элементы поисковика

    // формируем разделитель, содержащий каталог и таблицу
    splitter = split();

    // собственно, размещение
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(splitter);
    this->setLayout(layout);

    // помещаем на место кнопку сброса фильтра
    // button_position(0,0);
}

QSplitter *Searcher::split(){
    // формируем разделитель, содержащий каталог и таблицу
    QSplitter* splt = new QSplitter(Qt::Horizontal);
    splt->addWidget(this->catalog);
    splt->addWidget(this->table);

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
    table->clear();
    table->setRowCount(0);
    QStringList columns = get_columns_list();
    QSqlQuery query;
    QString strSelect = apply_filters();
    int rows = 0;
    //qDebug() << strSelect;
    if(!query.exec(strSelect)){
        error("Ошибка", QString("Не удалось выполнить запрос:\n").append(strSelect));
        //return -1;
    }
    table->setColumnCount(columns.size());
    table->setHorizontalHeaderLabels(columns);
    table->horizontalHeader()->setMovable(true);

    QSqlRecord rec = query.record();
    QString val;
    int i = 0, j;
    QTableWidgetItem* tableItem = 0;

    while(query.next()){
        rows++;
        table->setRowCount(rows);
        j = 0;
        foreach(QString tmp, this->filters.columns_filter()){
            val = query.value(rec.indexOf(tmp)).toString();
            tableItem = new QTableWidgetItem(val);
            table->setItem(i,j,tableItem);
            j++;
        }
        i++;
    }
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
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

    if(group == 0)
        query = "SELECT " + columns.join(",") + " FROM " + TOVMARKS_TABLE + " ORDER BY id LIMIT 50";
    else if(group < ENLARGER)
        query = "SELECT " + columns.join(",") + " FROM " + TOVMARKS_TABLE + " WHERE subgroup_id=" + QString::number(group) + " ORDER BY id LIMIT 50";
    else
        query = "SELECT " + columns.join(",") + " FROM " + TOVMARKS_TABLE + " WHERE subgroup_id IN (SELECT id FROM " + SUBGROUPS_TABLE + " WHERE group_id=" + QString::number(group-ENLARGER) + ") ORDER BY id LIMIT 50";
    return query;
}
