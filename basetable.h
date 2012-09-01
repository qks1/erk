#ifndef BASETABLE_H
#define BASETABLE_H

#include <QWidget>
#include "helpers.h"
#include "switcher.h"
#include "constants.h"

/*
    Класс, реализующий таблицу с переключателем страниц.
    Базовый для конкретных реализаций,
    сам по себе не предполагает заполнения.
    Реализует таблицу с переключателем и функции для переключателя.
*/

class MyHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    MyHeaderView(Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = 0);

private:
    void mouseReleaseEvent(QMouseEvent *e);
signals:
    void mouse_release();
};

class BaseTable : public QWidget
{
    Q_OBJECT
public:
    explicit BaseTable(QWidget *parent = 0);
    
    void set_totals(int);
    int get_items_on_page();
    void set_multipage_mode();
    void restore_width(int index, int width);
    void restore_order();
    int last_resized_index;
    int last_resized_width;
    //int last_old_visual_index;
    //int last_new_visual_index;

protected:
    QTableView *table;                    // таблица
    Switcher *switcher;                   // переключатель страниц
    QString settings_section;
    QStringList column_names;
    QSettings *settings;
    MyHeaderView *hh;
    bool filled;
    void create_new_table();
    // ПЕРЕМЕННЫЕ ДЛЯ ПЕРЕКЛЮЧАТЕЛЯ:
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
    void restore_state();
    void save_width();


private:
    inline void layout();                 // расположить элементы
    void move_switcher();

    inline void custom_table();
    inline void base_connects();


signals:
    void limits_changed(pair);
    void limits_removed();
    void limits_restored();
    void section_resized(int, int);
    void section_moved();

protected slots:
    void change_page(int);
    void change_onpage(int);
    void remove_limits();
    void restore_limits();
    void column_width_changed(int,int,int);
    void column_moved(int,int,int);



private slots:
    void save_state();
    void save_order();
    void section_resized_slot();

public slots:
    
};

#endif // BASETABLE_H
