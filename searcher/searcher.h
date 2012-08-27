/*
  Класс поисковика.
  Отвечает главным образом за хранение и применение фильтров.
*/

#ifndef SEARCHER_H
#define SEARCHER_H

#include <QtGui>
#include <QtSql>
#include "filters.h"
#include "helpers.h"
#include "constants.h"
#include "whitesearcher.h"
#include "greysearcher.h"

class Searcher : public QWidget
{
    Q_OBJECT
public:
    explicit Searcher(QWidget *parent = 0);
    void close_func();
    
private:
    QStackedWidget *stack;
    WhiteSearcher *white_searcher;
    GreySearcher *grey_searcher;
    Filters *filters;

    inline void connects();
    inline void grey_connects();
    inline void set_layout();
    inline void filters_default();
    inline void grey_filters_default();
    void clear_grey_boxes();
    int size_of_select(QString);
    int size_of_select(QStringList);
    QStringList valid_strings(QStringList);
    //WhiteSearcher *ws =

signals:
    void fill(QSqlQuery, QStringList);
    void clear_text_signal();

private slots:
    void refresh_grey();
    void set_group_filter(int);
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
    void reset_params_filter();
    void switch_prices_filter();
    void switch_quantity_filter();
    void set_last_filter(int);
    void open_grey(int);
    void close_grey(QModelIndex);
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

public slots:

};

#endif // SEARCHER_H
