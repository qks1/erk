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
    -- white_id - число, по которому производится отбор по id в белом экране
    -- white_id_mode - >= или ==
    -- beginname - шаблон для поиска с начала
    -- parts - шаблоны для поиска по фрагментам
    -- white_sort_order - порядок сортировки в белом экране (список (для устойчивой сортировки),
                            с указанием имени столбца и порядка сортировки)
    -- params - список строк с параметрами (выбранными в селекторах)
    -- price - какие цены отбираем (нулевые, ненулевые, все)
    -- quantity - какое количество отбираем в белом экране

    -- grey_id - какой id отображаем в сером экране
    -- grey_nolimits
    -- grey_limits
    -- grey_sort_order
    -- grey_text_id - число, по которому производится отбор по id в сером экране
    -- grey_id_mode - >= или ==
    -- grey_beginname - шаблон для поиска с начала в сером экране
    -- grey_parts - шаблоны для поиска по фрагментам в сером экране
    -- grey_storage
    -- grey_rack
    -- grey_board
    -- grey_box
    -- grey_from_year
    -- grey_to_year
    -- grey_one_year
    -- grey_year_mode
    -- grey_insp
    -- grey_add_info
    -- grey_defect
    -- grey_category
    -- grey_string_category


    -- mode - режим (белый или серый экран)
    -- last_applied_filter - последний применённый фильтр

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
    void restore_limits();  // установить nolimits = false
    void set_white_id(int); // установить фильтр по id в "белом экране"
    void reset_white_id();  // очистить фильтр по id
    void set_beginname(QString);
    void reset_beginname();
    void set_white_id_mode(int);
    void set_parts(QStringList);
    void add_white_sort_column(SortingOrder);
    void set_param(int, QString);
    void reset_params();
    void switch_prices();
    void set_prices(int);
    void switch_quantity();
    void set_quantity(int);
    void set_mode(int);
    void set_last_filter(int);
    void set_grey_id(int);
    void set_grey_limits(pair);
    void remove_grey_limits();
    void restore_grey_limits();
    void add_grey_sort_column(SortingOrder);
    void reset_grey_id();
    void set_grey_text_id(int); // установить фильтр по id в "белом экране"
    void reset_grey_text_id();  // очистить фильтр по id
    void set_grey_beginname(QString);
    void reset_grey_beginname();
    void set_grey_id_mode(int);
    void set_grey_parts(QStringList);
    void set_grey_storage(QString);
    void set_grey_rack(QString);
    void set_grey_board(QString);
    void set_grey_box(QString);
    void set_grey_from_year(QString);
    void set_grey_to_year(QString);
    void set_grey_one_year(QString);
    void set_grey_year_mode(int);
    void set_grey_insp(int);
    void set_grey_add_info(QString);
    void set_grey_defect(QString);
    void set_grey_category(int);
    void set_grey_string_category(QString);

    void clear_grey_storage();
    void clear_grey_rack();
    void clear_grey_board();
    void clear_grey_box();
    void clear_grey_one_year();
    void clear_grey_from_year();
    void clear_grey_to_year();
    void clear_grey_insp();
    void clear_grey_add_info();
    void clear_grey_defect();
    void clear_grey_category();

    // функции, возвращающие значения фильтров:
    int group_filter();
    QStringList columns_filter();
    pair limits_filter();
    int begin();
    bool are_there_limits();
    int white_id_filter();
    QString beginname_filter();
    int white_id_mode_filter();
    QStringList parts_filter();
    QList<SortingOrder> white_sort_order_filter();
    QStringList params_filter();
    int prices_filter();
    int quantity_filter();
    int mode();
    int last_applied_filter();
    int grey_id_filter();
    pair grey_limits_filter();
    QList<SortingOrder> grey_sort_order_filter();
    int grey_begin();
    bool are_there_grey_limits();
    int grey_text_id_filter();
    QString grey_beginname_filter();
    int grey_id_mode_filter();
    QStringList grey_parts_filter();
    QString grey_storage_filter();
    QString grey_rack_filter();
    QString grey_board_filter();
    QString grey_box_filter();
    QString grey_from_year_filter();
    QString grey_to_year_filter();
    QString grey_one_year_filter();
    int grey_year_mode_filter();
    int grey_insp_filter();
    QString grey_add_info_filter();
    QString grey_defect_filter();
    int grey_category_filter();
    QString grey_string_category_filter();


// непосредственно значения фильтров - private
private:
    int group;              // по группам
    QStringList columns;    // по столбцам
    pair limits;            // начало и конец выборки
    bool nolimits;          // выбрать весь объём данных?
    int white_id;           // "белый" id
    short white_id_mode;    // режим фильтра по белому id
    QString beginname;      // шаблон для поиска с начала
    QStringList parts;
    QList<SortingOrder> white_sort_order;
    QStringList params;
    int prices;
    int quantity;
    int grey_id;
    int search_mode;
    int last_filter;
    pair grey_limits;
    bool grey_nolimits;
    QList<SortingOrder> grey_sort_order;
    int grey_text_id;
    int grey_id_mode;
    QString grey_beginname;
    QStringList grey_parts;
    QString grey_storage;
    QString grey_rack;
    QString grey_board;
    QString grey_box;
    QString grey_from_year;
    QString grey_to_year;
    QString grey_one_year;
    int grey_year_mode;
    int grey_insp;
    QString grey_add_info;
    QString grey_defect;
    int grey_category;
    QString grey_string_category;

};

#endif // FILTERS_H
