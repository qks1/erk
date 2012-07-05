#ifndef TABLE_H
#define TABLE_H

#include <QtGui>
#include <QtSql>
#include "helpers.h"
#include "switcher.h"

/*
    Класс, реализующий таблицу с переключателем страниц
*/

class Table : public QWidget
{
    Q_OBJECT
public:
    explicit Table(QWidget *parent = 0);

    void fill_table(QSqlQuery query, QStringList columns);       // функция заполнения таблицы

    // ПЕРЕМЕННЫЕ ДЛЯ ПЕРЕКЛЮЧАТЕЛЯ:
    int current_page;                       // текущая страница
    int items_on_page;                      // записей на странице
    Switcher *switcher;                     // переключатель страниц
    int total_pages;

private:
    QTableWidget *table;                    // таблица
    QPushButton *qpb;
    inline void layout();                   // расположить элементы
    void move_switcher();
    inline void connects();

    void resizeEvent(QResizeEvent *);           // реакция на изменение размеров окна


signals:


private slots:
    void change_page(int);

public slots:


};

#endif // TABLE_H
