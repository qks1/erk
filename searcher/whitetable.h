#ifndef WHITETABLE_H
#define WHITETABLE_H

#include <QtGui>
#include <QtSql>
#include "helpers.h"
//#include "switcher.h"
#include "constants.h"
#include "mytablemodel.h"
#include "basetable.h"

/*
    Класс, реализующий таблицу для поисковика в белом экране.
    Унаследован от базового класса таблицы.
*/

class WhiteTable : public BaseTable
{
    Q_OBJECT
public:
    explicit WhiteTable(QWidget *parent = 0);

    void fill(MyTableModel*, QStringList, int, Qt::SortOrder, bool);       // функция заполнения таблицы
    QVariant data (QModelIndex, int);
    void close_func();                  // функция, вызываемая при закрытии таблицы.

private:
    inline void connects();
    void save_state();
    void restore_state();

    bool filled;                                // флаг, заполнена ли таблица
    QStringList column_names;
    int sort_column;
    Qt::SortOrder sort_order;
    QSettings *settings;



signals:
    void sort_order_changed(int, Qt::SortOrder);
    void prices_clicked();
    void quantity_clicked();
    void right_click(int);
    void double_click(QModelIndex);

private slots:
    void header_right_click(QPoint);
    void table_right_click(QPoint);
    void column_width_changed(int,int,int);
    void column_moved(int,int,int);

public slots:
    void change_order(int, Qt::SortOrder);


public slots:

};

#endif // WHITETABLE_H
