/*
  Класс поисковика.
  Отвечает главным образом за хранение и применение фильтров.
*/

#ifndef SEARCHER_H
#define SEARCHER_H

#include <QtGui>
#include <QtSql>
#include "filters.h"
#include "../common/helpers.h"
#include "../common/constants.h"
#include "whitesearcher.h"
#include "greysearcher.h"
#include "managerreservewidget.h"
#include "docs/docslist.h"
#include "lists/contragentslist.h"
#include "docs/document.h"

class Searcher : public QWidget
{
    Q_OBJECT
public:
    explicit Searcher(ReserveStruct *rstruct,
                       ColumnsStruct *white_columns,
                       ColumnsStruct *grey_columns,
                       QWidget *parent = 0);
    void resize_all();
    void restore_white_width(int index, int width);
    void restore_grey_width(int index, int width);
    void restore_manager_reserve_width(int index, int width);
    void restore_white_order(int logical, int newvisual);
    void restore_grey_order(int logical, int newvisual);
    void restore_manager_reserve_order(int logical, int newvisual);
    int open_white_columns_list();
    int open_grey_columns_list();
    void hide_show_white_columns();
    void hide_show_grey_columns();
    int mode();
    bool success;
    void set_date();
    void refresh_white_table();
    void refresh_grey_table();
    void refresh_blue_table();
    void switch_reserves();
    void set_reserve_header();
    void set_reserve_contragent(int id, QString name);
    void clear_reserve_contragent();
    void switch_hidden();
    void open_reserves_list();

private:
    QStackedWidget *stack;
    WhiteSearcher *white_searcher;
    GreySearcher *grey_searcher;
    GreySearcher *blue_searcher;
    //ManagerReserveWidget *reserve;
    Filters *filters;
    Filters *old_grey_filters;
    //QWidget* prev_widget;
    QStack<QWidget*> prev_widgets;

    inline void connects();
    inline void grey_connects(GreySearcher*);
    inline void set_layout();
    inline void filters_default();
    inline void grey_filters_default();
    void clear_grey_boxes();
    int size_of_select(QString);
    int size_of_select(QStringList);
    bool need_blue;
    int current_reserve_doc;
    QStringList valid_strings(QStringList);

signals:
    void fill(QSqlQuery, QStringList);
    void clear_text_signal();
    void section_resized(int, int, QString);
    void section_moved(int, int, QString);
    void open_settings();
    void catalog_hides();
    void catalog_shows();
    void white_columns_changed();
    void grey_columns_changed();
    void need_white_refresh();
    void need_grey_refresh();
    void need_blue_refresh();
    void need_total_refresh();
    void rename_tab(QString, bool);
    void rename_tab(QString);
    void switch_reserve_signal();
    void contragent_returned(int, QString);
    void clear_reserve_contragent_signal();
    void switch_hidden_signal();

private slots:
    void open_grey(int, QString);
    void refresh_grey();
    void close_grey(QModelIndex);
    void open_blue(int, QString);
    void close_blue();
    void set_group_filter(int, QString);
    void reset_group_filter();
    void set_limits_filter(pair);
    void set_nolimits();
    void set_limits();
    void set_text_filter(int, int, QString);
    void reset_text_filters(int);
    void reset_white_text_filters();
    void reset_grey_text_filters();
    void change_sort_order(int, Qt::SortOrder);
    void button_sort(int);
    void set_param_filter(QString, int);
    void reset_param_filter(int);
    void reset_params_filter();
    void switch_prices_filter();
    void switch_quantity_filter();
    void set_last_filter(int);
    void set_grey_limits_filter(pair);
    void set_grey_nolimits();
    void set_grey_limits();
    void change_grey_sort_order(int, Qt::SortOrder);
    void change_grey_storage(QString);
    void change_grey_rack(QString);
    void change_grey_board(QString);
    void change_grey_box(QString);
    void reset_grey_boxes();
    void change_grey_one_year(QString, int);
    void change_grey_from_year(QString, int);
    void change_grey_to_year(QString, int);
    void reset_grey_years();
    void change_grey_insp(QString);
    void reset_grey_insp();
    void change_grey_add_info(QString);
    void change_grey_defect(QString);
    void change_grey_category(QString);
    void reset_add_boxes();
    void grey_reset_slot();
    void open_contragents(int, int);
    void close_current();
    void contragent_returned_slot(int, QString);
    void open_docslist();
    void open_doc(int);

public slots:
    void show_white_catalog();
    void hide_white_catalog();

};

#endif // SEARCHER_H
