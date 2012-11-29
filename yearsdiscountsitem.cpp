#include "yearsdiscountsitem.h"
#include "ui_yearsdiscountsitem.h"

YearsDiscountsItem::YearsDiscountsItem(double retail, double whole, QWidget *parent) :
    QFrame(parent)
{
    change_retail = true;
    change_whole = true;
    formed = false;

    year_label = new QLabel(this);
    retail_label = new QLabel(this);
    whole_label = new QLabel(this);
    retail_rub = new QLabel("р.", this);
    whole_rub = new QLabel("р.", this);
    plusminus_button = new QPushButton(this);
    percents_spinbox = new QSpinBox(this);
    manual_checkbox = new QCheckBox(this);

    year_label->setAlignment(Qt::AlignRight);

    QFont font;
    font.setPointSize(16);
    plusminus_button->setFont(font);
    percents_spinbox->setSuffix("%");
    percents_spinbox->setRange(0, 999);

    base_retail = retail;
    base_whole = whole;
    this->setFrameStyle(QFrame::Box | QFrame::Raised);
    //this->setStyleSheet("border-color:black; border-width: 5px;");
    // ресайз лейблов
    plusminus_button->setFixedWidth(plusminus_button->fontMetrics().width('_')+2);
    year_label->setFixedWidth(year_label->fontMetrics().width('_') * 5 + 2);
    retail_label->adjustSize();
    whole_label->adjustSize();
    plusminus_button->setFlat(true);

    percents_spinbox->setValue(0);
    retail_label->setText(QString::number(base_retail, 'f', 2));
    whole_label->setText(QString::number(base_whole, 'f', 2));

    // по умолчанию discount_mode = true, соответственно, на кнопке рисуем минус
    discount_mode = true;
    plusminus_button->setText("‒");

    // ресаааааайз
    year_label->adjustSize();
    year_label->setFixedWidth(year_label->fontMetrics().width('_')*6);
    plusminus_button->setFixedWidth(plusminus_button->fontMetrics().width('_') * 2);
    plusminus_button->setFixedHeight(plusminus_button->width());
    percents_spinbox->setFixedWidth(percents_spinbox->fontMetrics().width('_') * 8);
    retail_label->setFixedWidth(retail_label->fontMetrics().width('_')*(QString::number(base_retail, 'f', 2).length()));
    whole_label->setFixedWidth(retail_label->width());
    retail_rub->setFixedWidth(retail_rub->fontMetrics().width('_')*2);
    whole_rub->setFixedWidth(retail_rub->width());
    manual_checkbox->adjustSize();
    manual_checkbox->setFixedSize(manual_checkbox->size());

    // ой какие костыли
    int wheight = percents_spinbox->height() * 1.2;
    // геометриииииии
    int wwdd = 0;
    year_label->move(wwdd, (wheight-year_label->height())/2);
    wwdd += year_label->width();
    plusminus_button->move(wwdd, (wheight-plusminus_button->height())/2);
    wwdd += plusminus_button->width();
    percents_spinbox->move(wwdd, percents_spinbox->height()*0.1);
    wwdd += percents_spinbox->width()+2;
    retail_label->move(wwdd, year_label->y());
    wwdd += retail_label->width()+3;
    retail_rub->move(wwdd, year_label->y());
    wwdd += retail_rub->width()+6;
    whole_label->move(wwdd, year_label->y());
    wwdd += whole_label->width()+3;
    whole_rub->move(wwdd, year_label->y());
    wwdd += whole_rub->width()+3;
    manual_checkbox->move(wwdd, (wheight-manual_checkbox->height())/2);
    wwdd += manual_checkbox->width();
    //tidish_widget->setFixedWidth(wwdd);
    this->setFixedSize(wwdd+5, wheight);

    QObject::connect(plusminus_button, SIGNAL(clicked()), SLOT(change_mode()));
    QObject::connect(manual_checkbox, SIGNAL(clicked(bool)), SLOT(switch_manual(bool)));
    QObject::connect(percents_spinbox, SIGNAL(valueChanged(int)), SLOT(count_prices()));

    //this->adjustSize();
    //this->setFixedSize(this->size());
}

// setters
void YearsDiscountsItem::set_year(QString year){
    year_label->setText(year);
}

void YearsDiscountsItem::set_percents(int percents){
    percents_spinbox->setValue(percents);
}

void YearsDiscountsItem::set_retail(double price){
    retail_label->setText(QString::number(price, 'f', 2));
}

void YearsDiscountsItem::set_whole(double price){
    whole_label->setText(QString::number(price, 'f', 2));
}

void YearsDiscountsItem::set_manual(bool manual){
    manual_checkbox->setChecked(manual);
    switch_manual(manual);
}

void YearsDiscountsItem::set_plusminus(bool mode){
    this->discount_mode = mode;
    plusminus_button->setText(this->discount_mode ? "‒" : "+");
    // перерасчёт
    count_prices();
}

void YearsDiscountsItem::set_retail_mode(bool b){
    this->change_retail = b;
}

void YearsDiscountsItem::set_whole_mode(bool b){
    this->change_whole = b;
}

void YearsDiscountsItem::set_formed(){
    this->formed = true;
}

void YearsDiscountsItem::set_unformed(){
    this->formed = false;
}

// getters
QString YearsDiscountsItem::year(){
    return year_label->text();
}

int YearsDiscountsItem::percents(){
    return percents_spinbox->value();
}

double YearsDiscountsItem::retail_price(){
    return retail_label->text().toDouble();
}

double YearsDiscountsItem::whole_price(){
    return whole_label->text().toDouble();
}

bool YearsDiscountsItem::manual(){
    return manual_checkbox->isChecked();
}

bool YearsDiscountsItem::mode(){
    return discount_mode;
}


void YearsDiscountsItem::change_mode(){
    set_plusminus(!discount_mode);
}

void YearsDiscountsItem::count_prices(){
    int sign = (discount_mode ? -1 : 1);
    if(change_retail) retail_label->setText(QString::number(base_retail * (1 + sign*(percents_spinbox->value()/100.00)), 'f', 2));
    if(change_whole) whole_label->setText(QString::number(base_whole * (1 + sign*(percents_spinbox->value()/100.00)), 'f', 2));

}

void YearsDiscountsItem::switch_manual(bool checked){
    percents_spinbox->setEnabled(!checked);
    plusminus_button->setEnabled(!checked);
    if(formed){
        // сделать перерасчёт, если ручные скидки отключены
        if(!checked)
            count_prices();
        // а если включены, открыть диалог ввода
        else manual_edit();
    }
}

void YearsDiscountsItem::manual_edit(){
    ManualPricesDialog *mpd = new ManualPricesDialog(retail_label->text().toDouble(), whole_label->text().toDouble(), this);
    two_prices p = mpd->exec();
    retail_label->setText(QString::number(p.retail, 'f', 2));
    whole_label->setText(QString::number(p.whole, 'f', 2));
}

void YearsDiscountsItem::set_base_retail(double price){
    this->base_retail = price;
}

void YearsDiscountsItem::set_base_whole(double price){
    this->base_whole = price;
}

// координаты
int YearsDiscountsItem::year_label_x(){
    return year_label->x();
}

int YearsDiscountsItem::retail_label_x(){
    return retail_label->x();
}

int YearsDiscountsItem::whole_label_x(){
    return whole_label->x();
}

int YearsDiscountsItem::percents_spinbox_x(){
    return percents_spinbox->x();
}

int YearsDiscountsItem::manual_checkbox_x(){
    return manual_checkbox->x();
}

void YearsDiscountsItem::set_retail_enabled(bool enabled){
    retail_label->setEnabled(enabled);
    retail_rub->setEnabled(enabled);
    change_retail = enabled;
}

void YearsDiscountsItem::set_whole_enabled(bool enabled){
    whole_label->setEnabled(enabled);
    whole_rub->setEnabled(enabled);
    change_whole = enabled;
}


//---------------------------------------------------------------//

ManualPricesDialog::ManualPricesDialog(double retail, double whole, QWidget *parent) : QDialog(parent)
{
    old_retail = retail;
    old_whole = whole;

    prices.retail = retail;
    prices.whole = whole;

    this->retail = new QDoubleSpinBox();
    this->whole = new QDoubleSpinBox();
    this->retail->setMaximum(1000000);
    this->whole->setMaximum(1000000);
    this->retail->setValue(retail);
    this->whole->setValue(whole);
    this->retail->setSuffix(" р.");
    this->whole->setSuffix(" р.");

    this->accept_button = new QPushButton("Записать");
    this->reject_button = new QPushButton("Отменить");

    QLabel *retail_label = new QLabel("Розн.");
    QLabel *whole_label = new QLabel("Опт.");

    // layout
    QGridLayout *lt = new QGridLayout();
    lt->addWidget(retail_label, 0, 0);
    lt->addWidget(whole_label, 1, 0);
    lt->addWidget(this->retail, 0, 1);
    lt->addWidget(this->whole, 1, 1);
    lt->addWidget(this->accept_button, 0, 2);
    lt->addWidget(this->reject_button, 1, 2);

    this->setLayout(lt);
    this->adjustSize();
    this->setFixedSize(this->size());
    this->setWindowTitle("Ввести цены");

    QObject::connect(this->accept_button, SIGNAL(clicked()), SLOT(accept()));
    QObject::connect(this->reject_button, SIGNAL(clicked()), SLOT(reject()));
}

two_prices ManualPricesDialog::exec(){
    this->show();
    this->raise();
    //this->activateWindow();
    event_loop.exec();

    return prices;
}

void ManualPricesDialog::accept(){
    prices.retail = this->retail->value();
    prices.whole = this->whole->value();
    event_loop.exit();
    QDialog::accept();
}

void ManualPricesDialog::reject(){
    event_loop.exit();
    QDialog::reject();
}
