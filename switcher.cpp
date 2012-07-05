#include "switcher.h"
#include "constants.h"
#include <QtGui>

Switcher::Switcher(QWidget *parent) :
    QWidget(parent)
{
    // создаём объекты
    first = new QPushButton();
    prev = new QPushButton();
    current = new QLineEdit();
    total = new QLabel();
    next = new QPushButton();
    last = new QPushButton();
    items = new QLineEdit();
    onpage = new QLabel("на странице");


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


    // меняем размеры и размещаем их
    layout();

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


}

void Switcher::set_current_page(int page){
    current->setText(QString::number(page));
}

void Switcher::set_total_pages(QString str){
    total->setText(str);
}


void Switcher::layout(){
    // размещаем элементы горизонтально
    QHBoxLayout *lt = new QHBoxLayout();

    lt->setSpacing(0);
    lt->setMargin(0);

    int button_size = 16;
    int text_size = SMALL_FONT_SIZE*3;
    int spacing = 15;

    first->setFixedSize(button_size,button_size);
    prev->setFixedSize(button_size,button_size);
    next->setFixedSize(button_size,button_size);
    last->setFixedSize(button_size,button_size);
    current->setFixedSize(text_size,button_size);
    items->setFixedSize(text_size,button_size);
    total->setFixedWidth(text_size);
    onpage->setFixedWidth(onpage->width());

    lt->addWidget(first);
    lt->addWidget(prev);
    lt->addWidget(current);
    //lt->addSpacing(5);
    lt->addWidget(total);
    lt->addWidget(next);
    lt->addWidget(last);
    lt->addSpacing(spacing);
    lt->addWidget(items);
    //lt->addSpacing(5);
    lt->addWidget(onpage);

    this->setFixedSize(button_size*4+text_size*3+onpage->width()+spacing, button_size);
    this->setLayout(lt);
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

