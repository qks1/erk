#ifndef FILTERS_H
#define FILTERS_H

#include <QtGui>

/*  класс, описывающий фильтры для поисковика.
    Возможные фильтры:
    -- фильтр по группам (groups_filter) - целое число;
    -- фильтр по столбцам (columns_filter) - список строк
*/

class Filters
{
public:
    Filters();     // конструктор
    // функции, устанавливающие значения фильтров:
    void set_group_filter(int);
    void set_columns_filter(QStringList);
    void clear_columns();   // очистить фильтр по столбцам

    // функции, возвращающие значения фильтров:
    int group_filter();
    QStringList columns_filter();

// непосредственно значения фильтров - private
private:
    int group;              // по группам
    QStringList columns;    // по столбцам

};

#endif // FILTERS_H
