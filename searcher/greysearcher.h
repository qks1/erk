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
#include "constants.h"
#include "customcombobox.h"

class GreySearcher : public QWidget
{
    Q_OBJECT
public:
    explicit GreySearcher(QWidget *parent = 0);
    void fill_table(Filters*, bool);
    int get_original_index(QString);
    QString get_original_name(int);
    int input_id_combobox_value();
    void clear_text();


private:
    GreyTable *greytable;
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

    //BUTTONS
    QPushButton *new_button;
    QPushButton *edit_button;
    QPushButton *del_button;

    QStringList original_columns_names;
    QString tables_str;//, where_id_str, where_text_str, where_places_str, where_years_str, where_insp_str;
    QMap<QString, QString> where_strings;
    Filters *filters;
    bool filled;
    bool years_box_filled;

    void set_layout();
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
    
signals:
    void close_grey(QModelIndex);
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

public slots:
    
};

class TableModelForYears : public QSqlQueryModel{
    Q_OBJECT
public:
    explicit TableModelForYears(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
};

#endif // GREYSEARCHER_H
