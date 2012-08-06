/*
  Класс поисковика.
  Отвечает главным образом за хранение и применение фильтров.
  При смене любого фильтра необходимо перезаполнить активную таблицу,
  поэтому вызывается функция fill_table().
*/

#ifndef SEARCHER_H
#define SEARCHER_H

#include <QtGui>
#include <QtSql>
#include "filters.h"
#include "helpers.h"
#include "switcher.h"
#include "catalog.h"
#include "table.h"
#include "constants.h"
#include "input.h"
#include "paramsselector.h"

class Searcher : public QAbstractScrollArea
{
    Q_OBJECT
public:
    explicit Searcher(QWidget *parent = 0);
    bool ok;                                // чтобы другие виджеты могли проверить, всё ли нормально
    
private:
    Table *white_table;                     // таблица "белого экрана"
    Table *grey_table;                      // таблицы "серого экрана"
    Table *active_table;                    // какая таблица в данный момент активна
    Catalog *catalog;                       // каталог групп
    Input *input;                           // текстовое поле с радиокнопками
    QList<ParamsSelector*> selectors;       // набор выпадающих списков с параметрами
    QPushButton *params_reset;              // кнопка сброса параметров
    int max_params;                         // сколько селекторов надо отобразить
    Filters *filters;                       // набор фильтров (см. класс Filters)
    QList<QStringList> params_lists;        // набор списков параметров, из которых будут формироваться чекбоксы
    int single_group;                       // если фильтры обеспечивают выбор только из одной подгруппы, здесь хранится её номер, иначе -1
    int single_group_without_limits;        // то же, что single_group, только без учёта limits_filter
    QMap<QString, QString> column_names;
    QMap<QString, QString> params_names;
    QStringList original_column_names;
    QMap<QString, QString> get_params_names(int);
    QString rename_column(QString, int group);
    void set_button_labels();


    QString apply_filters();                // функция "применить фильтры", формирующая строку запроса к БД
    QSplitter *split();                     // функция установки разделителя
    inline void set_layout();               // функция размещения объектов
    inline void connects();                 // список соединений сигналов и слотов
    inline void initialize();               // инициализация переменных
    inline void fill_column_names();        // заполняем хэш column_names
    void resizeEvent(QResizeEvent *);       // реакция на изменение размеров окна

    QString list_to_string(QStringList);
    int size_of_select(QString);
    int size_of_select(QStringList);
    QStringList valid_strings(QStringList);
    QStringList sql_to_list(QSqlQuery);


    int set_totals(QString);
    void get_params_list(QString, QString);
    void fill_boxes();
    void fill_table(bool, bool);

    int last_applied_filter;                // последний применённый фильтр. Нужен, чтобы если при вводе символа
                                            // в текстовое поле результат содержит ноль строк - распознать, что
                                            // был применён текстовый фильтр, и удалить последний символ

    QString sorting_order_to_string(QList<SortingOrder>);


    // ПЕРЕМЕННЫЕ:


signals:
    void fill(QSqlQuery, QStringList);

private slots:
    void set_group_filter(int);
    void reset_group_filter();
    void set_limits_filter(pair);
    void set_nolimits();
    void set_limits();
    void set_text_filter(int, QString);
    void reset_text_filters();
    void change_sort_order(int, Qt::SortOrder);
    void button_sort(int);
    void set_param_filter(QString, int);
    void reset_params_filter();
    void switch_prices_filter();

public slots:

};

#endif // SEARCHER_H
