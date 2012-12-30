#ifndef GREYSEARCHER_H
#define GREYSEARCHER_H

/*
   Поисковик в сером экране.
   Содержит таблицу и две панели.
   Верхняя - кнопки действий (добавить, удалить, редактировать и т.д.
   Нижняя - комбобоксы с фильтрами по местам на складе,
   по году (с возможностью выбора - фиксированный или диапазон),
   по приёмке.
*/

#include <QtGui>
#include <QtSql>
#include "greytable.h"
#include "catalog.h"
#include "input.h"
#include "paramsselector.h"
#include "filters.h"
#include "../common/constants.h"
#include "../common/customcombobox.h"
#include "../common/helpers.h"
#include "greyadddialog.h"
#include "greymovedialog.h"
#include "managerreservewidget.h"
#include "storagereservewidget.h"

class GreySearcher : public QWidget
{
    Q_OBJECT
public:
    explicit GreySearcher(ReserveStruct *rstruct,
                          bool need_blue,
                          bool blue,
                          ColumnsStruct *columns,
                          QWidget *parent = 0);
    void fill_table(Filters*, bool);
    void open_trademark(int id, Filters *f);
    int get_original_index(QString);
    QString get_original_name(int);
    int input_id_combobox_value();
    void clear_text();
    void restore_width(int index, int width);
    void restore_order(int logical, int newvisual);
    void restore_manager_reserve_width(int index, int width);
    void restore_manager_reserve_order(int logical, int newvisual);
    int open_columns_list();
    void hide_show_columns();
    void switch_reserve();
    void set_reserve_header();
    void set_reserve_contragent(int id, QString name);
    void clear_reserve_contragent();

private:
    GreyTable *grey_table;
    Input *input;
    CustomComboBox *storages;
    CustomComboBox *racks;
    CustomComboBox *boards;
    CustomComboBox *boxes;
    QPushButton *reset_boxes;
    CustomComboBox *one_year;
    CustomComboBox *from_year;
    CustomComboBox *to_year;
    QLabel *from_year_label;
    QLabel *to_year_label;
    QPushButton *reset_years_button;
    QRadioButton *oneyear_mode;
    QRadioButton *multiyear_mode;
    QComboBox *insp_box;
    QPushButton *reset_insp_button;
    // для доп. параметров
    QRadioButton *add_info_mode;
    QRadioButton *defect_mode;
    QRadioButton *category_mode;
    CustomComboBox *add_info_box;
    CustomComboBox *defect_box;
    CustomComboBox *category_box;
    QPushButton *reset_add_boxes;
    ManagerReserveWidget *reserve;
    StorageReserveWidget *pipe;

    // охохо
    bool blue;
    bool need_blue;
    int items_status;

    // размеры всякие
    int panel_button_size;
    int input_width;
    int reset_button_size;
    int spacing;
    int selector_width;

    bool edit_prices_permission;

    //BUTTONS
    QHBoxLayout *buttons_lt;
    QMenu *edit_button_menu;
    QPushButton *new_button;
    QPushButton *edit_button;
    QPushButton *delete_button;
    QPushButton *reset_button;
    QPushButton *prices_button;
    QPushButton *docs_button;
    QPushButton *columns_button;
    QPushButton *settings_button;
    QPushButton *escape_button;
    QPushButton *reserve_button;
    QPushButton *blue_button;

    QStringList original_columns_names;
    QString tables_str;
    QMap<QString, QString> where_strings;
    Filters *filters;
    bool filled;
    bool years_box_filled;
    int last_added_place;

    // функции для лайаута
    void set_layout();
    QWidget *create_top_panel();
    QWidget *create_bottom_panel();
    QWidget *create_years_layout();
    QWidget *create_add_layout();

    void connects();
    int set_totals(QString);
    QVBoxLayout *create_box_layout(QComboBox*, QString, int);
    bool fill_places_box(int);
    bool fill_year_box(int mode);
    void set_years_filters();
    bool set_place_filter(int);
    void set_storages_filter();
    void set_insp_filter();
    void set_add_filter();
    void clear_boxes();
    void clear_where_strs();
    void fill_insp_box();
    void show_add_info_box();
    void show_defect_box();
    void show_category_box();
    QString year_text(QString);
    QString glue_where(QStringList *ex = 0);
    QString glue_where(QString);
    QString apply_filters();
    QMenu *create_edit_button_menu();

    
signals:
    void close_grey(QModelIndex);
    void open_blue_signal(int, QString);
    void close_blue_signal();
    void last_filter_changed(int);
    void limits_changed(pair);
    void limits_removed();
    void limits_restored();
    void sort_order_changed(int, Qt::SortOrder);
    void text_changed_signal(int, int, QString);
    void reset_signal();
    void storage_signal(QString);
    void rack_signal(QString);
    void board_signal(QString);
    void box_signal(QString);
    void reset_boxes_signal();
    void just_change_place(QString, QString, int, QString);
    void change_one_year_signal(QString, int);
    void change_from_year_signal(QString, int);
    void change_to_year_signal(QString, int);
    void refresh();
    void reset_years();
    void change_insp_signal(QString);
    void reset_insp_signal();
    void change_add_info_signal(QString);
    void change_defect_signal(QString);
    void change_category_signal(QString);
    void reset_add_boxes_signal();
    void total_reset_signal();
    void section_resized(int, int, QString);
    void section_moved(int, int, QString);
    void columns_changed();
    void open_settings();
    void switch_reserve_signal();
    void need_refresh();
    void need_blue_refresh();
    void refresh_searcher();
    void open_contragents(int,int);
    void clear_contragent();

private slots:
    void text_changed_slot(int, QString);
    void show_second_box();
    void hide_second_box();
    void change_mode();
    void one_year_changed(QString);
    void from_year_changed(QString);
    void to_year_changed(QString);
    void fill_one_year_box();
    void fill_from_year_box();
    void fill_to_year_box();
    void fill_storages_box();
    void fill_racks_box();
    void fill_boards_box();
    void fill_boxes_box();
    void reset_boxes_slot();
    void change_add_mode();
    void fill_add_info_box();
    void fill_defect_box();
    void fill_category_box();
    void storages_slot(QString);
    void racks_slot(QString);
    void boards_slot(QString);
    void boxes_slot(QString);
    // штукуёвины для кнопок
    void add_button_slot();
    void edit_button_slot();
    void delete_button_slot();
    void reset_button_slot();
    void prices_button_slot();
    void docs_button_slot();
    void columns_button_slot();
    void settings_button_slot();
    void escape_button_slot();
    void edit_correction_slot(bool moving = false);
    void edit_chargeoff_slot();
    void edit_move_slot();
    void reserve_button_slot();
    void open_blue_slot();
    void set_last_place(int id);

public slots:
    void refresh_table();
    
};

class TableModelForYears : public QSqlQueryModel{
    Q_OBJECT
public:
    explicit TableModelForYears(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
};


#endif // GREYSEARCHER_H
