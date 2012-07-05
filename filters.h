#ifndef FILTERS_H
#define FILTERS_H

#include <QtGui>

/*  класс, описывающий фильтры для поисковика.
    Возможные фильтры:
    -- фильтр по группам (groups_filter) - целое число;
    -- фильтр по столбцам (columns_filter) - список строк
    -- limit - начало и конец выборки (переменные begin и end)
*/

class Filters
{
public:
    Filters();     // конструктор
    // функции, устанавливающие значения фильтров:
    void set_group_filter(int);
    void set_columns_filter(QStringList);
    void clear_columns();   // очистить фильтр по столбцам
    void set_begin(int);
    void set_end(int);

    // функции, возвращающие значения фильтров:
    int group_filter();
    QStringList columns_filter();
    int begin_value();
    int end_value();

// непосредственно значения фильтров - private
private:
    int group;              // по группам
    QStringList columns;    // по столбцам
    int begin;              // начало выборки
    int end;                // конец выборки

};

#endif // FILTERS_H
