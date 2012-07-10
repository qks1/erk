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

/*
  Класс поисковика.
  Отвечает главным образом за хранение и применение фильтров.
  При смене любого фильтра необходимо перезаполнить активную таблицу,
  поэтому вызывается функция fill_table().
*/

class Searcher : public QWidget
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
    QSplitter *splitter;                    // разделитель каталога и таблицы
    Filters *filters;                       // набор фильтров (см. класс Filters)
    QString apply_filters();                // функция "применить фильтры", формирующая строку запроса к БД
    QStringList get_columns_list();         // формирует список столбцов для таблицы
    QSplitter *split();                     // функция установки разделителя
    inline void layout();                   // функция размещения объектов
    inline void connects();                 // список соединений сигналов и слотов
    inline void initialize();               // инициализация переменных
    void resizeEvent(QResizeEvent *);       // реакция на изменение размеров окна

    void set_totals(QString);
    void fill_table(bool reset_groups);


    // ПЕРЕМЕННЫЕ:


signals:
    void fill(QSqlQuery, QStringList);

private slots:
    void set_group_filter(int);
    void set_limits_filter(pair);
    void set_nolimits();
    void set_limits();

public slots:

};

#endif // SEARCHER_H
