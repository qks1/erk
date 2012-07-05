#ifndef SWITCHER_H
#define SWITCHER_H

#include <QtGui>

/*
  Класс переключателя страниц для таблицы.
*/

class switcher : public QWidget
{
    Q_OBJECT
public:
    explicit switcher(QWidget *parent = 0);

private:
    // таблица, которой управляет данный переключатель
    // QTableWidget *table;

    // элементы слева направо:
    QPushButton *first;         // кнопка "На первую страницу"
    QPushButton *prev;          // кнопка "На предыдущую страницу"
    QLineEdit *current;         // текстовое поле "На страницу №"
    QLabel *total;              // надпись "всего страниц"
    QPushButton *next;          // кнопка "На следующую страницу"
    QPushButton *last;          // кнопка "На последнюю страницу"
    QLineEdit *items;           // текстовое поле "элементов на странице"
    QLabel *onpage;             // надпись "элементов на странице"

    void layout();

signals:
    
public slots:
    
};

#endif // SWITCHER_H
