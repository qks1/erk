#ifndef WHITESEARCHER_H
#define WHITESEARCHER_H

#include <QWidget>
#include <QtSql>
#include "whitetable.h"
#include "catalog.h"
#include "input.h"
#include "paramsselector.h"
#include "filters.h"
#include "../common/constants.h"
#include "../common/detailcard.h"
#include "whiteadddialog.h"
#include "../common/helpers.h"
#include "whitepriceseditor.h"
#include "addyeardiscount.h"
#include "yearsdiscountsdialog.h"
#include "yearsdiscounts.h"
#include "quickquantitywindow.h"
#include "managerreservewidget.h"
#include "storagereservewidget.h"
#include "docs/docslist.h"
#include "reserveslist.h"

/*
    Поисковик в белом экране.
    Основная часть состоит из каталога и белой таблицы (WhiteTable).
    Также есть селекторы (комбобокс+кнопка) по числу параметров,
    текстовое поле с радиокнопками и кнопки действий (добавление, редактирование и т.д.).
*/

class WhiteSearcher : public QWidget
{
    Q_OBJECT
public:
    explicit WhiteSearcher(ReserveStruct *rstruct,
                           bool need_blue,
                           ColumnsStruct *columns,
                           QStackedWidget *stack,
                           QStack<QWidget*> prev_wgts,
                           QWidget *parent = 0);
    QStackedWidget *stack;
    QStack<QWidget*> prev_wgts;
    void fill_table(Filters*, bool, bool);              // заполнение таблицы
    void clear_text();
    int input_id_combobox_value();
    int get_original_index(QString);
    QString get_original_name(int);
    void resize_all();
    void restore_width(int index, int width);
    void restore_order(int logical, int newvisual);
    void restore_manager_reserve_width(int index, int width);
    void restore_manager_reserve_order(int logical, int newvisual);
    int open_columns_list();
    void hide_show_columns();
    bool success;
    bool edit_prices_permission;
    void set_date();
    void switch_reserve();
    void switch_hidden_public();
    void set_reserve_header();
    void set_reserve_contragent(int id, QString name);
    void clear_reserve_contragent();
    void open_reserves_list();



private:
    // ВИДЖЕТЫ:
    WhiteTable *white_table;                // таблица "белого экрана"
    Catalog *catalog;                       // каталог групп
    Input *input;                           // текстовое поле с радиокнопками
    QList<ParamsSelector*> selectors;       // набор выпадающих списков с параметрами
    QPushButton *params_reset;              // кнопка сброса параметров
    YearsDiscounts *dlg;
    WhitePricesEditor *wpe;
    ManagerReserveWidget *reserve;
    StorageReserveWidget *pipe;

    // КНОПКИ НА ВЕРХНЕЙ ПАНЕЛИ
    QHBoxLayout *buttons_lt;
    QPushButton *new_button;
    QPushButton *edit_button;
    QPushButton *delete_button;
    QPushButton *prices_button;
    QPushButton *discounts_button;
    QPushButton *photo_button;
    QPushButton *docs_button;
    QPushButton *catalog_button;
    QPushButton *settings_button;
    QPushButton *reserve_button;
    QPushButton *blue_button;

    // СПИСКИ И ПЕРЕМЕННЫЕ:
    QList<QStringList> params_lists;        // набор списков параметров, из которых будут формироваться чекбоксы
    QMap<QString, QString> params_names;    // имена параметров, отображаемые в заголовках соотв. столбцов и на кнопках селекторов
    int single_group;                       // если фильтры обеспечивают выбор только из одной подгруппы, здесь хранится её номер, иначе -1
    int single_group_without_limits;        // то же, что single_group, только без учёта limits_filter
    QStringList original_column_names;      // оригинальные имена столбцов (для получения индекса)
    bool boxes_filled;                      // заполнены ли комбобоксы с параметрами
    QString tables_str;
    QMap<QString, QString> where_strings;
    Filters *filters;
    int panel_button_size;
    int current_row;
    bool need_blue;
    bool grey_mode;
    QStandardItemModel *reserves_list_model;
    Document *opened_doc;
    DocsList *opened_docslist;

    // ФУНКЦИИ ДЛЯ КОНСТРУКТОРА:
    inline void set_layout();               // функция размещения объектов
    inline void white_connects();           // список соединений сигналов и слотов

    // ФУНКЦИИ:
    QString apply_filters(Filters*);                        // функция "применить фильтры", формирующая строку запроса к БД
    QMap<QString, QString> get_params_names(int);           // получить названия параметров
    void set_button_labels();                               // устанавливает названия кнопок над селекторами (с подсчётом элементов)
    void set_button_labels(QSqlQuery);
    void set_text_button_labels();                          // устанавливает названия кнопок над селекторами (без подсчёта)
    void clear_button_labels();                             // очищает названия кнопок над селекторами
    QString rename_column(QString, int group);              // переименовывает столбцы с названием "par№_val" в №
    int set_totals(QString);                                // считает кол-во записей
    QSplitter *split();                                     // функция установки разделителя
    QString glue_where(QStringList *ex = 0);
    QString glue_where(QString);
    void clear_where_strs();
    bool up_selected_string();
    bool down_selected_string();
    void load_default_picture(QPixmap*);

signals:
    void fill(QSqlQuery, QStringList);
    void last_filter_changed(int);
    void group_changed(int, QString);
    void limits_changed(pair);
    void limits_removed();
    void limits_restored();
    void text_changed_signal(int, int, QString);
    void reset_signal();
    void sort_order_changed(int, Qt::SortOrder);
    void item_selected(QString, int);
    void button_clicked(int);
    void reset_param_signal(int);
    void params_reset_clicked();
    void prices_clicked();
    void quantity_clicked();
    void create_grey(int, QString);
    void create_blue(int, QString);
    void section_resized(int, int, QString);
    void section_moved(int, int, QString);
    void open_settings();
    void catalog_shows();
    void catalog_hides();
    void columns_changed();
    void need_refresh();
    void rename_tab(QString);
    void switch_reserve_signal();
    void refresh_searcher();
    void open_contragents(int, int);
    void clear_contragent();
    void open_docslist();
    void switch_hidden_signal();

private slots:
    void detail_info(int);
    void fill_box(int index);

    void open_grey(QModelIndex);
    void text_changed_slot(int, QString);
    void reset_params();
    void new_detail_slot();
    void edit_detail_slot();
    void delete_detail_slot();
    void edit_prices_slot();
    void edit_discounts_slot();
    void open_photo_slot();
    void open_docs_window();
    void switch_catalog();
    void up_years_discounts();
    void down_years_discounts();
    void up_prices_editor();
    void down_prices_editor();
    void reset_table();
    void reserve_button_slot();
    void switch_grey_mode();
    void close_current();
    void create_move_dialog_slot(Document*);
    void move_to_doc(int);
    void move_to_reserve(int);
    void set_reserve_doc_data_slot(int id);


public slots:
    void hide_catalog();
    void show_catalog();
    void refresh_table();

};

#endif // WHITESEARCHER_H

