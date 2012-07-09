#ifndef FILTERS_H
#define FILTERS_H

#include <QtGui>
#include "helpers.h"

/*  класс, описывающий фильтры для поисковика.
    Возможные фильтры:
    -- фильтр по группам (groups_filter) - целое число;
    -- фильтр по столбцам (columns_filter) - список строк
    -- limits - начало и конец выборки (пара limits)
    -- nolimits - булева переменная. Если true, то значение limits игнорируется и должен выбираться весь объём данных

*/

class Filters
{
public:
    Filters();     // конструктор
    // функции, устанавливающие значения фильтров:
    void set_group_filter(int);
    void set_columns_filter(QStringList);
    void clear_columns();   // очистить фильтр по столбцам
    void set_limits(pair);
    void remove_limits();   // установить nolimits = true
    void restore_limits();   // установить nolimits = false

    // функции, возвращающие значения фильтров:
    int group_filter();
    QStringList columns_filter();
    pair limits_filter();
    int begin();
    bool are_there_limits();

// непосредственно значения фильтров - private
private:
    int group;              // по группам
    QStringList columns;    // по столбцам
    pair limits;            // начало и конец выборки
    bool nolimits;          // выбрать весь объём данных?

};

#endif // FILTERS_H
