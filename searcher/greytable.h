#ifndef GREYTABLE_H
#define GREYTABLE_H

#include <QtGui>
#include <QtSql>
#include "basetable.h"
#include "constants.h"
#include "helpers.h"
#include "mytablemodel.h"

class GreyTableModel : public MyTableModel
{
    Q_OBJECT
public:
    explicit GreyTableModel(QWidget *parent = 0);
    QColor background_color;

    QVariant data(const QModelIndex &index, int role) const;
};

class BlueTableModel : public GreyTableModel
{
    Q_OBJECT
public:
    explicit BlueTableModel(QWidget *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
};


/*
    Класс, реализующий таблицу для поисковика в сером экране.
    Унаследован от базового класса таблицы.
*/

class GreyTable : public BaseTable
{
    Q_OBJECT
public:
    explicit GreyTable(QString settings_section,
                       bool need_switcher,
                       bool need_multiselect,
                       bool blue,
                       ColumnsStruct *columns,
                       QWidget *parent = 0);
    ~GreyTable();

    void fill(GreyTableModel*, QStringList, int, Qt::SortOrder, bool);       // функция заполнения таблицы
    void close_func();                  // функция, вызываемая при закрытии таблицы.

private:
    inline void connects();

    bool blue;
    int sort_column;
    Qt::SortOrder sort_order;
    QSettings *settings;

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);


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
