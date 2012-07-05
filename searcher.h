#ifndef SEARCHER_H
#define SEARCHER_H

#include <QtGui>
#include "filters.h"
#include "helpers.h"
#include "switcher.h"
#include "catalog.h"

/*
  Класс поисковика.
*/

class Searcher : public QWidget
{
    Q_OBJECT
public:
    explicit Searcher(QWidget *parent = 0);
    
private:
    QTableWidget *table;                    // таблица "белого экрана"
    Catalog *catalog;                       // каталог групп
    QSplitter *splitter;                    // разделитель каталога и таблицы
    Filters filters;                        // набор фильтров (см. класс Filters)
    QString apply_filters();                // функция "применить фильтры", формирующая строку запроса к БД
    QStringList get_columns_list();         // формирует список столбцов для таблицы
    void fillTable();                       // заполняет таблицу
    QSplitter *split();                     // функция установки разделителя
    inline void layout();                   // функция размещения объектов
    inline void connects();                 // список соединений сигналов и слотов
    inline void initialize();               // инициализация переменных
    void resizeEvent(QResizeEvent *);       // реакция на изменение размеров окна

    switcher *swtch;

    // ПЕРЕМЕННЫЕ:


signals:

private slots:
    void set_group_filter(int);

public slots:

};

#endif // SEARCHER_H
