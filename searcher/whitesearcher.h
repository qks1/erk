#ifndef WHITESEARCHER_H
#define WHITESEARCHER_H

#include <QWidget>
#include <QtSql>
#include "whitetable.h"
#include "catalog.h"
#include "input.h"
#include "paramsselector.h"
#include "filters.h"
#include "constants.h"
#include "detailcard.h"

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
    explicit WhiteSearcher(QWidget *parent = 0);
    void fill_table(Filters*, bool, bool);              // заполнение таблицы
    void clear_text();
    void close_func();
    int input_id_combobox_value();
    int get_original_index(QString);
    QString get_original_name(int);


private:
    // ВИДЖЕТЫ:
    WhiteTable *white_table;                // таблица "белого экрана"
    Catalog *catalog;                       // каталог групп
    Input *input;                           // текстовое поле с радиокнопками
    QList<ParamsSelector*> selectors;       // набор выпадающих списков с параметрами
    QPushButton *params_reset;              // кнопка сброса параметров

    // СПИСКИ И ПЕРЕМЕННЫЕ:
    QList<QStringList> params_lists;        // набор списков параметров, из которых будут формироваться чекбоксы
    QMap<QString, QString> params_names;    // имена параметров, отображаемые в заголовках соотв. столбцов и на кнопках селекторов
    int single_group;                       // если фильтры обеспечивают выбор только из одной подгруппы, здесь хранится её номер, иначе -1
    int single_group_without_limits;        // то же, что single_group, только без учёта limits_filter
    QStringList original_column_names;      // оригинальные имена столбцов (для получения индекса)
    bool boxes_filled;                      // заполнены ли комбобоксы с параметрами
    QString tables_string, where_string;    // для выбора параметров

    // ФУНКЦИИ ДЛЯ КОНСТРУКТОРА:
    inline void set_layout();               // функция размещения объектов
    inline void white_connects();           // список соединений сигналов и слотов

    // ФУНКЦИИ:
    QString apply_filters(Filters*);                        // функция "применить фильтры", формирующая строку запроса к БД
    void get_params_list();                                 // заполнение комбобоксов с параметрами
    QMap<QString, QString> get_params_names(int);           // получить названия параметров
    void set_button_labels();                               // устанавливает названия кнопок над селекторами (с подсчётом элементов)
    void set_button_labels(QSqlQuery);
    void set_text_button_labels();                          // устанавливает названия кнопок над селекторами (без подсчёта)
    void clear_button_labels();                             // очищает названия кнопок над селекторами
    QString rename_column(QString, int group);              // переименовывает столбцы с названием "par№_val" в №
    int set_totals(QString);                                // считает кол-во записей
    QSplitter *split();                                     // функция установки разделителя

signals:
    void fill(QSqlQuery, QStringList);
    void last_filter_changed(int);
    void group_changed(int);
    void limits_changed(pair);
    void limits_removed();
    void limits_restored();
    void text_changed_signal(int, int, QString);
    void reset_signal();
    void sort_order_changed(int, Qt::SortOrder);
    void item_selected(QString, int);
    void button_clicked(int);
    void params_reset_clicked();
    void prices_clicked();
    void quantity_clicked();
    void create_grey(int);

private slots:
    void detail_info(int);
    void fill_boxes_slot();
    void open_grey(QModelIndex);
    void text_changed_slot(int, QString);

public slots:
    
};

#endif // WHITESEARCHER_H
