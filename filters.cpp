#include "filters.h"

// конструктор, в котором присваиваем значения фильтров по умолчанию
Filters::Filters(){
    group = 0;
}

// установить значение group_filter, переданное в параметре
void Filters::set_group_filter(int g){
    this->group = g;
}

// очистить columns_filter
void Filters::clear_columns(){
    this->columns.clear();
}

// установить значение columns_filter, переданное в параметре
void Filters::set_columns_filter(QStringList c){
    // предварительно необходимо очистить его
    this->columns.clear();
    this->columns = c;
}

// установить значение переменной limits
void Filters::set_limits(pair l){
    this->limits = l;
}

void Filters::remove_limits(){
    this->nolimits = true;
}

void Filters::restore_limits(){
    this->nolimits = false;
}

void Filters::set_white_id(int id){
    this->white_id = id;
}

void Filters::reset_white_id(){
    this->white_id = 0;
}

void Filters::set_beginname(QString name){
    this->beginname = name;
}

void Filters::reset_beginname(){
    this->beginname = "";
}

void Filters::set_white_id_mode(int mode){
    this->white_id_mode = mode;
}

void Filters::set_parts(QStringList parts){
    this->parts = parts;
}

void Filters::add_white_sort_column(SortingOrder item){
    int index;
    // если столбец уже есть в списке, удаляем его и вставляем в начало
    if((index = this->white_sort_order.indexOf(item)) >= 0){
        this->white_sort_order.removeAt(index);
    }
    this->white_sort_order.push_front(item);
}

void Filters::set_param(int num, QString str){
    this->params[num] = str;
}

void Filters::reset_params(){
    this->params.clear();
    for(int i = 0; i < MAX_PARAMS; i++)
        params << "";
}



// вернуть значение group_filter
int Filters::group_filter(){
    return this->group;
}

// вернуть значение columns_filter
QStringList Filters::columns_filter(){
    return this->columns;
}

// вернуть значение limits
pair Filters::limits_filter(){
    return this->limits;
}

// вернуть начало выборки
int Filters::begin(){
    return this->limits.begin;
}

bool Filters::are_there_limits(){
    return (nolimits ? false : true);
}

// вернуть "белый" id
int Filters::white_id_filter(){
    return this->white_id;
}

// вернуть шаблон дял поиска с начала
QString Filters::beginname_filter(){
    return this->beginname;
}

int Filters::white_id_mode_filter(){
    return this->white_id_mode;
}

QStringList Filters::parts_filter(){
    return this->parts;
}

QList<SortingOrder> Filters::white_sort_order_filter(){
    return this->white_sort_order;
}

QStringList Filters::params_filter(){
    return this->params;
}
