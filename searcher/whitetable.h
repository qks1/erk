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

class WhiteTableModel : public MyTableModel{
    Q_OBJECT
public:
    explicit WhiteTableModel(QWidget *parent = 0);

    QDate retail_change_date;
    void set_date();

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void sort(int column, Qt::SortOrder order);

};

class WhiteTable : public BaseTable
{
    Q_OBJECT
public:
    explicit WhiteTable(QWidget *parent = 0);
    ~WhiteTable();

    void fill(WhiteTableModel*, QStringList, int, Qt::SortOrder, bool);       // функция заполнения таблицы
    QVariant data (QModelIndex, int);
    void hide_show_columns();
    void set_date();

private:
    void restore_state();
    inline void connects();
    int sort_column;
    Qt::SortOrder sort_order;



signals:
    void sort_order_changed(int, Qt::SortOrder);
    void prices_clicked();
    void quantity_clicked();
    void right_click(int);
    void double_click(QModelIndex);

private slots:
    void header_right_click(QPoint);
    void table_right_click(QPoint);

public slots:
    void change_order(int, Qt::SortOrder);


public slots:

};



#endif // WHITETABLE_H
