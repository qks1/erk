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

// вернуть значение group_filter
int Filters::group_filter(){
    return this->group;
}

// вернуть значение columns_filter
QStringList Filters::columns_filter(){
    return this->columns;
}
