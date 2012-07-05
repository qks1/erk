#include "switcher.h"
#include <QtGui>

switcher::switcher(QWidget *parent) :
    QWidget(parent)
{
    // создаём объекты
    first = new QPushButton("|<");
    prev = new QPushButton("<");
    current = new QLineEdit();
    total = new QLabel("из ");
    next = new QPushButton(">");
    last = new QPushButton(">|");
    items = new QLineEdit();
    onpage = new QLabel("на странице");

    // меняем размеры и размещаем их
    layout();
    this->resize(150,20);
}


void switcher::layout(){
    // размещаем элементы горизонтально
    QHBoxLayout *lt = new QHBoxLayout();

    lt->addWidget(first);
    lt->addWidget(prev);
    lt->addWidget(current);
    lt->addWidget(total);
    lt->addWidget(next);
    lt->addWidget(last);
    lt->addWidget(items);
    lt->addWidget(onpage);

    // устанавливаем размеры полей ввода
    current->resize(50,20);
    items->resize(50,20);
}
