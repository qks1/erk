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

// установить значение переменной begin
void Filters::set_begin(int value){
    this->begin = value;
}

// установить значение переменной end
void Filters::set_end(int value){
    this->end = value;
}

// вернуть значение group_filter
int Filters::group_filter(){
    return this->group;
}

// вернуть значение columns_filter
QStringList Filters::columns_filter(){
    return this->columns;
}

// вернуть значение begin
int Filters::begin_value(){
    return this->begin;
}

// вернуть значение end
int Filters::end_value(){
    return this->end;
}
