#include "paramsselector.h"

ParamsSelector::ParamsSelector(int number, QWidget *parent) :
    QWidget(parent)
{
    this->button = new QPushButton;
    this->items = new QComboBox;
    this->number = number;

    this->items->setEditable(true);
    this->items->setAutoCompletion(true);
    this->items->setInsertPolicy(QComboBox::NoInsert);

    set_layout();
    connect();
}

inline void ParamsSelector::connect(){
    QObject::connect(this->items, SIGNAL(activated(QString)),
                     this, SLOT(select_item(QString)));
}

inline void ParamsSelector::set_layout(){
    QVBoxLayout *lt = new QVBoxLayout;
    lt->setSpacing(0);
    lt->setMargin(0);
    lt->addWidget(button);
    lt->addWidget(items);
    this->button->setFixedHeight(this->button->fontMetrics().height() + 4);
    this->setFixedHeight(button->height() + items->height());

    this->setLayout(lt);
}

void ParamsSelector::set_button_label(QString text){
    this->button->setText(text);
}

void ParamsSelector::add_item(QString item){
    this->items->addItem(item);
}

void ParamsSelector::clear_items(){
    this->items->clear();
}

void ParamsSelector::set_selected(int index){
    this->items->setCurrentIndex(index);
}



void ParamsSelector::select_item(QString item){
    emit(item_selected(item, this->number));
}
