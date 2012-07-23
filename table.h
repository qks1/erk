#ifndef TABLE_H
#define TABLE_H

#include <QtGui>
#include <QtSql>
#include "helpers.h"
#include "switcher.h"
#include "constants.h"
#include "mytablemodel.h"

/*
    Класс, реализующий таблицу с переключателем страниц.
*/

class Table : public QWidget
{
    Q_OBJECT
public:
    explicit Table(QWidget *parent = 0);

    void set_totals(int);
    int get_items_on_page();
    void set_multipage_mode();


    void fill(MyTableModel*, QString, Qt::SortOrder, int, bool);       // функция заполнения таблицы

private:
    QTableView *table;                    // таблица
    QPushButton *qpb;
    inline void layout();                   // расположить элементы
    void move_switcher();
    inline void connects();

    // ПЕРЕМЕННЫЕ ДЛЯ ПЕРЕКЛЮЧАТЕЛЯ:
    Switcher *switcher;                     // переключатель страниц
    int current_page;                       // текущая страница
    int items_on_page;                      // записей на странице
    int total_pages;
    int total_items;
    int begin;
    // при смене кол-ва записей на странице первая запись должна остаться той же.
    // поэтому номер первой записи не всегда будет кратным числу записей на странице.
    // в переменной offset хранится этот самый сдвиг.
    int offset;
    void update_switcher_values();

    void resizeEvent(QResizeEvent *);           // реакция на изменение размеров окна

    QMap<QString, QString> column_names;
    QMap<QString, QString> params_names;
    QStringList original_column_names;
    QMap<QString, QString> get_params_names(int);
    QString rename_column(QString, int group);
    QString sort_column;
    Qt::SortOrder sort_order;



signals:
    void limits_changed(pair);
    void limits_removed();
    void limits_restored();
    void sort_order_changed(QString, Qt::SortOrder);

private slots:
    void change_page(int);
    void change_onpage(int);
    void remove_limits();
    void restore_limits();
    void change_order(int, Qt::SortOrder);


public slots:


};

#endif // TABLE_H
