#ifndef BASETABLEVIEW_H
#define BASETABLEVIEW_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QDebug>
#include "helpers.h"
#include "constants.h"
#include "helpers.h"


class SignalHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    SignalHeaderView(Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = 0);

private:

protected:
    void mouseReleaseEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

signals:
    void mouse_released();
    void mouse_pressed();
};


class BaseTableView : public QTableView
{
    Q_OBJECT
public:
    BaseTableView(QString settings_section,
                  bool need_multiselect,
                  ColumnsStruct *columns,
                  QList<int> default_widths,
                  QWidget *parent = 0);
    BaseTableView(QWidget *parent = 0);
    ~BaseTableView();

    // это для столбцов. Что-то, может, и не нужно, потом почистить
    void restore_width(int index, int width);
    void restore_columns_width();
    void restore_order(int logical, int newvisual);
    void restore_columns_order();
    void hide_show_columns();
    void restore_state();
    int open_columns_list();
    int resized_index;
    int resized_width;
    int old_index;
    int old_width;
    bool need_moving;
    ColumnsStruct *columns;
    QList<int> default_ws;

    // это вообще на будущее
    void resize_all();

    // это для обновления таблицы
    bool refresh_table();
    void set_last_query(QString);
    void clear_last_query();

    QString current_name(QString);
    QVariant table_data(int column);
    QVariant table_data(int row, int column, Qt::ItemDataRole role = Qt::DisplayRole);
    QVariant table_multi_data(int column);
    int selection_size();
    void delete_model();
    void set_filled();
    void set_variables(QString settings_section,
                       bool need_multiselect,
                       ColumnsStruct *columns,
                       QList<int> default_widths);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *event);
    QString settings_section;             // как это будет обзываться в настройках. Передаётся в конструкторе!
    QStringList column_names;
    QSettings *settings;
    SignalHeaderView *hh;
    QDialog *clist;
    QListWidget *list;
    QMap<QString,QString> original_names;
    QString last_query;
    bool filled;

private:
    bool need_multiselect;
    void resize_row();

    inline void custom_table();
    inline void set_header();
    inline void create_connects();

signals:
    void section_resized(int, int, QString);
    void section_moved(int, int, QString);
    void mouse_pressed(QPoint);
    void need_refresh();
    //void clicked(QModelIndex);
    void double_clicked(QModelIndex);

protected slots:
    void column_width_changed(int,int,int);
    void column_moved(int,int,int);

private slots:
    void mouse_released_slot();
    void mouse_pressed_slot();
    void accept_clist();
    void reject_clist();
    void catch_mouse_pressed(QPoint);

};

#endif // BASETABLEVIEW_H
