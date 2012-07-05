#ifndef SWITCHER_H
#define SWITCHER_H

#include <QtGui>

/*
  Класс переключателя страниц для таблицы.
*/

class Switcher : public QWidget
{
    Q_OBJECT
public:
    explicit Switcher(QWidget *parent = 0);

    void set_current_page(int);
    void set_total_pages(QString);

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
    inline void connects();

signals:
    void page_changed(int);

private slots:
    // слоты, срабатывающие при нажатии на кнопку переключения
    void first_clicked();
    void prev_clicked();
    void next_clicked();
    void last_clicked();
    
public slots:
    
};

#endif // SWITCHER_H
