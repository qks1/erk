#ifndef BASETABLE_H
#define BASETABLE_H

#include <QWidget>
#include <QSqlQueryModel>
#include "helpers.h"
#include "switcher.h"
#include "constants.h"
#include "helpers.h"

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

protected:
    void mouseReleaseEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);

signals:
    void mouse_released();
    void mouse_pressed();
};

 ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class SmartTableView : public QTableView
{
    Q_OBJECT
public:
    SmartTableView(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *e);

signals:
    void mouse_pressed(QPoint);
};

 ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class BaseTable : public QWidget
{
    Q_OBJECT
public:
    explicit BaseTable(QString settings_section,
                       bool need_switcher,
                       bool need_multiselect,
                       ColumnsStruct *columns,
                       QWidget *parent = 0);
    ~BaseTable();
    
    // это для свитчера
    void set_totals(int);
    int get_items_on_page();
    void set_multipage_mode();

    // это для столбцов. Что-то, может, и не нужно, потом почистить
    void restore_width(int index, int width);
    void restore_columns_width();
    void restore_order(int logical, int newvisual);
    void hide_show_columns();
    int open_columns_list();
    int resized_index;
    int resized_width;
    int old_index;
    int old_width;
    bool need_moving;
    ColumnsStruct *columns;

    // это вообще на будущее
    void resize_all();

    // это для обновления таблицы
    bool refresh_table();
    void set_last_query(QString);
    void clear_last_query();

    QString current_name(QString);
    QVariant table_data(int column);
    QVariant table_data(QString name);
    QVariant table_multi_data(int column);
    int current_row();
    void set_current_row(int);
    int row_count();
    int selection_size();
    void delete_model();


protected:
    SmartTableView *table;                // таблица
    Switcher *switcher;                   // переключатель страниц
    QString settings_section;             // как это будет обзываться в настройках. Передаётся в конструкторе!
    QStringList column_names;
    QSettings *settings;
    MyHeaderView *hh;
    QDialog *clist;
    QListWidget *list;
    QMap<QString,QString> original_names;
    QString last_query;
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
    void wheelEvent(QWheelEvent *);
    void restore_state();
    void restore_columns_order();
    void save_width();
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);


private:
    bool need_switcher;
    bool need_multiselect;
    inline void layout();                 // расположить элементы
    void move_switcher();
    void resize_row();

    inline void custom_table();
    inline void base_connects();


signals:
    void limits_changed(pair);
    void limits_removed();
    void limits_restored();
    void section_resized(int, int, QString);
    void section_moved(int, int, QString);
    void need_refresh();

protected slots:
    void change_page(int);
    void change_onpage(int);
    void remove_limits();
    void restore_limits();
    void column_width_changed(int,int,int);
    void column_moved(int,int,int);



private slots:
    void mouse_released_slot();
    void mouse_pressed_slot();
    void accept_clist();
    void reject_clist();
    void catch_mouse_pressed(QPoint);

public slots:
    
};

#endif // BASETABLE_H
