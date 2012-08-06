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
    QObject::connect(this->button, SIGNAL(clicked()),
                     this, SLOT(click_button()));
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

void ParamsSelector::set_button_label(QString text, int count = -1){
    //if(count != )
    // посчитаем ширину надписи (count)
    int count_width = this->button->fontMetrics().width("(" + QString::number(count) + ")");

    // посчитаем ширину текста (плюс пробел)
    int text_width = this->button->fontMetrics().width(text + " ");

    if(this->button->width() < text_width + count_width + 10){
        // считаем, сколько места осталось на текст (с учётом многоточия)
        int remaining = this->button->width() - (this->button->fontMetrics().width("... ") + count_width + 10);
        // и теперь помещаем столько букв, сколько поместится
        QString short_text = "";
        QChar c;
        int used = 0, i = 0;
        do{
            used += this->button->fontMetrics().width(text[i]);
            short_text += text[i];
            i++;
        } while(used <= remaining && i < text.size());
        this->button->setText(short_text + "... (" + QString::number(count) + ")");
    }
    else
        this->button->setText(text + " (" + QString::number(count) + ")");

    this->button->setToolTip(text + " (" + QString::number(count) + ")");
}

void ParamsSelector::add_item(QString item){
    this->items->addItem(item);
}

void ParamsSelector::add_items(QStringList items){
    this->items->addItems(items);
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

void ParamsSelector::setEnabled(bool enabled){
    this->button->setText("");
    this->button->setEnabled(enabled);
    this->items->setEnabled(enabled);
}

void ParamsSelector::setModel(QSqlQueryModel* model){
    this->items->setModel(model);
}

void ParamsSelector::setModelColumn(int c){
    this->items->setModelColumn(c);
}

int ParamsSelector::count(){
    return this->items->count();
}

void ParamsSelector::click_button(){
    emit(button_clicked(this->number));
}
