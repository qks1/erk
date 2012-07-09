#include "filters.h"
#include <QtGui>

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

