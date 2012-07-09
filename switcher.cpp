#include "switcher.h"
#include "constants.h"
#include <QtGui>

//--------------------------------------------------------------------------//
//--------------------------- КОНСТРУКТОР-----------------------------------//
//--------------------------------------------------------------------------//

Switcher::Switcher(QWidget *parent) :
    QWidget(parent)
{
    // СОЗДАЁМ ОБЪЕКТЫ:
    // layout
    switcher_layout = new QHBoxLayout;

    // элементы для постраничного режима
    first = new QPushButton();
    prev = new QPushButton();
    current = new QLineEdit();
    total_pages = new QLabel();
    next = new QPushButton();
    last = new QPushButton();
    items = new QLineEdit();
    items_on_page = new QLabel("на странице / ");
    show_all = new QPushButton();

    // элементы для одоностраничного режима
    total_items = new QLabel();
    switch_to_pages = new QPushButton("Постранично");
    // для кнопок items_on_page и switch_to_pages устанавливаем текст прямо в конструкторе, т.к. меняться он не будет

    // режим по умолчанию - постраничный
    mode = MULTIPAGE_MODE;



    // размеры по умолчанию (что есть что - см. в хедере)
    button_size = 16;
    total_label_size = SMALL_FONT_SIZE*4;
    line_edit_size = SMALL_FONT_SIZE*3;
    big_button_size = SMALL_FONT_SIZE*7;

    // иконки для кнопок переключения страниц
    QPixmap first_icon("images/first.png");
    QPixmap prev_icon("images/prev.png");
    QPixmap next_icon("images/next.png");
    QPixmap last_icon("images/last.png");

    // устанавливаем их
    first->setIcon(first_icon);
    prev->setIcon(prev_icon);
    next->setIcon(next_icon);
    last->setIcon(last_icon);

    set_fonts();
    set_sizes();

    // меняем размеры элементов и размещаем их
    set_layout();

    // устанавливаем непрозрачный фон
    QPalette p;
    p.setColor(this->backgroundRole(), Qt::white);
    this->setPalette(p);
    this->setAutoFillBackground(true);

    // соединяем сигналы со слотами
    connects();
}

inline void Switcher::connects(){
    // при нажатии на кнопку переключения страницы - послать сигнал page_changed
    QObject::connect(this->first, SIGNAL(clicked()),
                     this, SLOT(first_clicked()));
    QObject::connect(this->prev, SIGNAL(clicked()),
                     this, SLOT(prev_clicked()));
    QObject::connect(this->next, SIGNAL(clicked()),
                     this, SLOT(next_clicked()));
    QObject::connect(this->last, SIGNAL(clicked()),
                     this, SLOT(last_clicked()));

    // при вводе номера страницы - послать сигнал page_changed
    QObject::connect(this->current, SIGNAL(returnPressed()),
                     this, SLOT(page_entered()));

    // при вводе кол-ва элементов на странице - послать сигнал items_on_page_changed
    QObject::connect(this->items, SIGNAL(returnPressed()),
                     this, SLOT(items_on_page_entered()));

    // при нажатии на кнопку "все на одной странице" - послать сигнал "singlepage_mode_selected"
    QObject::connect(this->show_all, SIGNAL(clicked()),
                     this, SLOT(singlepage_mode_clicked()));

    // при нажатии на кнопку "постранично" - послать сигнал "multipage_mode_selected"
    QObject::connect(this->switch_to_pages, SIGNAL(clicked()),
                     this, SLOT(multipage_mode_clicked()));


}

inline void Switcher::background(){
    QPalette p;
    p.setColor(this->backgroundRole(), Qt::white);
    this->setPalette(p);
    this->setAutoFillBackground(true);
}

inline void Switcher::set_sizes(){
    first->setFixedSize(button_size,button_size);
    prev->setFixedSize(button_size,button_size);
    next->setFixedSize(button_size,button_size);
    last->setFixedSize(button_size,button_size);
    current->setFixedSize(line_edit_size,button_size);
    items->setFixedSize(line_edit_size,button_size);
    total_pages->setFixedWidth(total_label_size);
    items_on_page->adjustSize();
    items_on_page->setFixedWidth(items_on_page->width());
    show_all->setFixedSize(big_button_size,button_size);

    total_items->setFixedSize(big_button_size, button_size);
    switch_to_pages->setFixedSize(big_button_size, button_size);
}

inline void Switcher::set_fonts(){
    // в переключателе должен использоваться шрифт меньший, чем в остальной программе.
    QFont lineFont;
    lineFont.setPixelSize(SMALL_FONT_SIZE);

    // установим шрифт на элементы: кнопки (кроме тех, что с иконками), текстовые поля, лейблы
    current->setFont(lineFont);
    items->setFont(lineFont);
    total_pages->setFont(lineFont);
    items_on_page->setFont(lineFont);
    show_all->setFont(lineFont);
    total_items->setFont(lineFont);
    switch_to_pages->setFont(lineFont);
}

inline void Switcher::set_layout(){
    switcher_layout->setMargin(0);
    switcher_layout->setSpacing(0);

    if(mode == MULTIPAGE_MODE){
        switcher_layout->addWidget(first);
        switcher_layout->addWidget(prev);
        switcher_layout->addWidget(current);
        switcher_layout->addWidget(total_pages);
        switcher_layout->addWidget(next);
        switcher_layout->addWidget(last);
        //switcher_layout->addSpacing(spacing);
        switcher_layout->addWidget(items);
        switcher_layout->addWidget(items_on_page);
        switcher_layout->addWidget(show_all);

        this->setFixedSize(button_size*4+line_edit_size*2+total_label_size+items_on_page->width()+big_button_size, button_size);
    }
    else if(mode == SINGLEPAGE_MODE){
        switcher_layout->addWidget(total_items);
        switcher_layout->addWidget(switch_to_pages);

        this->setFixedSize(big_button_size*2, button_size);

    }
    this->setLayout(switcher_layout);
}

void Switcher::clear_layout(){
    /*
    QLayoutItem *item;
    while((item = switcher_layout->takeAt(0)) != 0){
        switcher_layout->removeItem(item);
        item->widget()->hide();
    }
    */

    /*
      Естественно, надо бы использовать закомментированную часть.
      Но она почему-то работает не всегда корректно, посему да, мы не ищем лёгких путей.
    */
    if(this->mode == MULTIPAGE_MODE){
        switcher_layout->removeWidget(first);
        switcher_layout->removeWidget(prev);
        switcher_layout->removeWidget(current);
        switcher_layout->removeWidget(total_pages);
        switcher_layout->removeWidget(next);
        switcher_layout->removeWidget(last);
        switcher_layout->removeWidget(items);
        switcher_layout->removeWidget(items_on_page);
        switcher_layout->removeWidget(show_all);
        first->hide();
        prev->hide();
        current->hide();
        total_pages->hide();
        next->hide();
        last->hide();
        items->hide();
        items_on_page->hide();
        show_all->hide();
    }
    else if(this->mode = SINGLEPAGE_MODE){
        switcher_layout->removeWidget(total_items);
        switcher_layout->removeWidget(switch_to_pages);
        total_items->hide();
        switch_to_pages->hide();
    }

    delete switcher_layout;
}


void Switcher::set_current_page(int page){
    current->setText(QString::number(page));
}

void Switcher::set_show_all(int items){
    show_all->setText("Все " + QString::number(items));
}

void Switcher::set_total_pages(int pages){
    total_pages->setText("из " + QString::number(pages));
}

void Switcher::set_items_on_page(int iop){
    items->setText(QString::number(iop));
}

void Switcher::set_total_items_label(int items){
    total_items->setText("Позиций: " + QString::number(items));
}



void Switcher::switch_to_singlepage_mode(){
    if(this->mode == MULTIPAGE_MODE){
        this->clear_layout();

        switcher_layout = new QHBoxLayout;
        this->mode = SINGLEPAGE_MODE;
        this->set_layout();

        total_items->show();
        switch_to_pages->show();
    }
}

void Switcher::switch_to_multipage_mode(){
    if(this->mode == SINGLEPAGE_MODE){
        this->clear_layout();
        switcher_layout = new QHBoxLayout;
        this->mode = MULTIPAGE_MODE;
        this->set_layout();

        first->show();
        prev->show();
        current->show();
        total_pages->show();
        next->show();
        last->show();
        items->show();
        items_on_page->show();
        show_all->show();
    }
}

int Switcher::page_number(){
    return current->text().toInt();
}

//--------------------------------------------------------------------------//
//--------------------------- СЛОТЫ ----------------------------------------//
//--------------------------------------------------------------------------//

void Switcher::first_clicked(){
    emit page_changed(PAGE_FIRST);
}

void Switcher::prev_clicked(){
    emit page_changed(PAGE_PREV);
}

void Switcher::next_clicked(){
    emit page_changed(PAGE_NEXT);
}

void Switcher::last_clicked(){
    emit page_changed(PAGE_LAST);
}

void Switcher::page_entered(){
    emit(page_changed(PAGE_NUMBER));
}

void Switcher::items_on_page_entered(){
    emit items_on_page_changed(items->text().toInt());
}

void Switcher::singlepage_mode_clicked(){
    emit singlepage_mode_selected();
}

void Switcher::multipage_mode_clicked(){
    emit multipage_mode_selected();
}

