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
    void set_total_pages(int);
    void set_total_items(int);
    void set_items_on_page(int);

    void set_total_items_label(int);

    void switch_to_onepage_mode();
    void switch_to_page_mode();

    int page_number();

private:
    // текущий режим (постраничный или все элементы на одной странице)
    short mode;

    // элементы слева направо:
    // для постраничного режима:
    QHBoxLayout *page_layout;
    QPushButton *first;             // кнопка "На первую страницу"
    QPushButton *prev;              // кнопка "На предыдущую страницу"
    QLineEdit *current;             // текстовое поле "На страницу №"
    QLabel *total;                  // надпись "всего страниц"
    QPushButton *next;              // кнопка "На следующую страницу"
    QPushButton *last;              // кнопка "На последнюю страницу"
    QLineEdit *items;               // текстовое поле "элементов на странице"
    QLabel *onpage;                 // надпись "элементов на странице"
    QPushButton *all;               // кнопка "Показать все"

    // для одностраничного режима
    QLabel *tot_items;              // надпись "%count% элементов"
    QPushButton *switch_to_pages;   // кнопка переключения на постраничный режим

    inline void set_layout();
    inline void set_onepage_layout();
    inline void set_sizes();
    inline void background();
    inline void connects();

    void clear_layout();

    int button_size;
    int total_label_size;
    int line_edit_size;
    int big_button_size;
    int spacing;


signals:
    void page_changed(int);
    void onpage_changed(int);
    void all_selected();
    void page_mode_selected();

private slots:
    // слоты, срабатывающие при нажатии на кнопку переключения
    void first_clicked();
    void prev_clicked();
    void next_clicked();
    void last_clicked();

    // слот, срабатывающий при вводе кол-ва элементов на странице
    void onpage_entered();

    // слот, срабатывающий при вводе номера страницы
    void page_entered();

    // слот, срабатывающий при нажатии на кнопку "все на одной странице"
    void all_clicked();

    // слот, срабатывающий при нажатии на кнопку "постранично"
    void page_mode_clicked();
    
public slots:
    
};

#endif // SWITCHER_H
