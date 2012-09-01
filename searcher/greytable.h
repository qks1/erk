#ifndef GREYTABLE_H
#define GREYTABLE_H

#include <QtGui>
#include <QtSql>
#include "basetable.h"
#include "constants.h"
#include "helpers.h"
#include "mytablemodel.h"

/*
    Класс, реализующий таблицу для поисковика в сером экране.
    Унаследован от базового класса таблицы.
*/

class GreyTable : public BaseTable
{
    Q_OBJECT
public:
    explicit GreyTable(QWidget *parent = 0);

    void fill(MyTableModel*, QStringList, int, Qt::SortOrder, bool);       // функция заполнения таблицы
    void close_func();                  // функция, вызываемая при закрытии таблицы.

private:
    inline void connects();

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
    void table_right_click(QPoint);

public slots:
    void change_order(int, Qt::SortOrder);


public slots:

};

#endif // GREYTABLE_H
