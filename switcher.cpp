#include "switcher.h"
#include "constants.h"
#include <QtGui>

Switcher::Switcher(QWidget *parent) :
    QWidget(parent)
{
    // создаём объекты
    page_layout = new QHBoxLayout;
    first = new QPushButton();
    prev = new QPushButton();
    current = new QLineEdit();
    total = new QLabel();
    next = new QPushButton();
    last = new QPushButton();
    items = new QLineEdit();
    onpage = new QLabel("на странице / ");
    all = new QPushButton();

    tot_items = new QLabel();
    switch_to_pages = new QPushButton("Постранично");

    mode = 1;

    button_size = 16;
    total_label_size = SMALL_FONT_SIZE*4;
    line_edit_size = SMALL_FONT_SIZE*3;
    big_button_size = SMALL_FONT_SIZE*7;
    spacing = 10;


    // иконки для переключателя страниц
    QPixmap first_icon("images/first.png");
    QPixmap prev_icon("images/prev.png");
    QPixmap next_icon("images/next.png");
    QPixmap last_icon("images/last.png");

    first->setIcon(first_icon);
    prev->setIcon(prev_icon);
    next->setIcon(next_icon);
    last->setIcon(last_icon);

    QFont lineFont;
    lineFont.setPixelSize(10);

    current->setFont(lineFont);
    items->setFont(lineFont);
    total->setFont(lineFont);
    onpage->setFont(lineFont);
    all->setFont(lineFont);
    tot_items->setFont(lineFont);
    switch_to_pages->setFont(lineFont);

    set_sizes();
    //set_onepage_layout();

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

    // при вводе кол-ва элементов на странице - послать сигнал onpage_changed
    QObject::connect(this->items, SIGNAL(returnPressed()),
                     this, SLOT(onpage_entered()));

    // при нажатии на кнопку "все на одной странице" - послать сигнал "all_selected"
    QObject::connect(this->all, SIGNAL(clicked()),
                     this, SLOT(all_clicked()));

    // при нажатии на кнопку "постранично" - послать сигнал "page_mode_selected"
    QObject::connect(this->switch_to_pages, SIGNAL(clicked()),
                     this, SLOT(page_mode_clicked()));


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
    total->setFixedWidth(total_label_size);
    onpage->adjustSize();
    onpage->setFixedWidth(onpage->width());
    all->setFixedSize(big_button_size,button_size);

    tot_items->setFixedSize(big_button_size, button_size);
    switch_to_pages->setFixedSize(big_button_size, button_size);
}

inline void Switcher::set_layout(){
    page_layout->setMargin(0);
    page_layout->setSpacing(0);

    if(mode == 1){
        page_layout->addWidget(first);
        page_layout->addWidget(prev);
        page_layout->addWidget(current);
        page_layout->addWidget(total);
        page_layout->addWidget(next);
        page_layout->addWidget(last);
        //page_layout->addSpacing(spacing);
        page_layout->addWidget(items);
        page_layout->addWidget(onpage);
        page_layout->addWidget(all);

        this->setFixedSize(button_size*4+line_edit_size*2+total_label_size+onpage->width()+big_button_size, button_size);
    }
    else if(mode == 2){
        page_layout->addWidget(tot_items);
        page_layout->addWidget(switch_to_pages);

        this->setFixedSize(big_button_size*2, button_size);

    }
    this->setLayout(page_layout);
}

void Switcher::clear_layout(){
    /*
    QLayoutItem *item;
    while((item = page_layout->takeAt(0)) != 0){
        page_layout->removeItem(item);
        item->widget()->hide();
    }
    */

    /*
      Естественно, надо бы использовать закомментированную часть.
      Но она почему-то работает не всегда корректно, посему да, мы не ищем лёгких путей.
    */
    if(this->mode == 1){
        page_layout->removeWidget(first);
        page_layout->removeWidget(prev);
        page_layout->removeWidget(current);
        page_layout->removeWidget(total);
        page_layout->removeWidget(next);
        page_layout->removeWidget(last);
        page_layout->removeWidget(items);
        page_layout->removeWidget(onpage);
        page_layout->removeWidget(all);
        first->hide();
        prev->hide();
        current->hide();
        total->hide();
        next->hide();
        last->hide();
        items->hide();
        onpage->hide();
        all->hide();
    }
    else{
        page_layout->removeWidget(tot_items);
        page_layout->removeWidget(switch_to_pages);
        tot_items->hide();
        switch_to_pages->hide();
    }

    delete page_layout;
}


void Switcher::set_current_page(int page){
    current->setText(QString::number(page));
}

void Switcher::set_total_items(int items){
    all->setText("Все " + QString::number(items));
}

void Switcher::set_total_pages(int pages){
    total->setText("из " + QString::number(pages));
}

void Switcher::set_items_on_page(int iop){
    items->setText(QString::number(iop));
}

void Switcher::set_total_items_label(int items){
    tot_items->setText("Позиций: " + QString::number(items));
}



void Switcher::switch_to_onepage_mode(){
    if(this->mode == 1){
        this->clear_layout();

        page_layout = new QHBoxLayout;
        this->mode = 2;
        this->set_layout();

        tot_items->show();
        switch_to_pages->show();
    }
}

void Switcher::switch_to_page_mode(){
    if(this->mode == 2){
        this->clear_layout();
        page_layout = new QHBoxLayout;
        this->mode = 1;
        this->set_layout();

        first->show();
        prev->show();
        current->show();
        total->show();
        next->show();
        last->show();
        items->show();
        onpage->show();
        all->show();
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

void Switcher::onpage_entered(){
    emit onpage_changed(items->text().toInt());
}

void Switcher::all_clicked(){
    emit all_selected();
}

void Switcher::page_mode_clicked(){
    emit page_mode_selected();
}

