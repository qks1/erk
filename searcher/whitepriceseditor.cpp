#include "whitepriceseditor.h"
#include "ui_whitepriceseditor.h"

WhitePricesEditor::WhitePricesEditor(int id, QString name, double price_ret, double price_whole, int whole_begin, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WhitePricesEditor)
{
    ui->setupUi(this);
    this->setWindowTitle("Редактирование цен");
    // сохраняем значения из конструктора
    this->trademark_id = id;
    this->trademark_name = name;
    this->base_price_ret = price_ret;
    this->base_price_whole = price_whole;
    this->old_whole_begin = whole_begin;

    ui->plus_button->setChecked(true);

    // подставляем значения в форму
    fill_values();
    set_name_label();
    enable_percents(false);

    QFont tfont;
    tfont.setBold(true);
    tfont.setItalic(true);
    ui->name_label->setFont(tfont);

    QObject::connect(ui->percents_checkbox, SIGNAL(clicked(bool)), SLOT(enable_percents(bool)));
    QObject::connect(ui->percents_spinbox, SIGNAL(valueChanged(int)), SLOT(percents_changed(int)));
    QObject::connect(ui->plus_button, SIGNAL(clicked()), SLOT(percents_changed()));
    QObject::connect(ui->minus_button, SIGNAL(clicked()), SLOT(percents_changed()));
    QObject::connect(ui->up_write_button, SIGNAL(clicked()), SLOT(apply_up()));
    QObject::connect(ui->down_write_button, SIGNAL(clicked()), SLOT(apply_down()));
    QObject::connect(ui->up_button, SIGNAL(clicked()), SLOT(move_up()));
    QObject::connect(ui->down_button, SIGNAL(clicked()), SLOT(move_down()));
    QObject::connect(ui->close_button, SIGNAL(clicked()), SLOT(reject()));
}

WhitePricesEditor::~WhitePricesEditor()
{
    delete ui;
}

void WhitePricesEditor::renew(int id, QString name, double price_ret, double price_whole, int whole_begin){
    // сохраняем значения из конструктора
    this->trademark_id = id;
    this->trademark_name = name;
    this->base_price_ret = price_ret;
    this->base_price_whole = price_whole;
    this->old_whole_begin = whole_begin;

    ui->plus_button->setChecked(true);

    // подставляем значения в форму
    ui->retail_base->setText(QString::number(base_price_ret, 'f', 2));
    ui->whole_base->setText(QString::number(base_price_whole, 'f', 2));

    set_name_label();
    enable_percents(false);
}


void WhitePricesEditor::fill_values(){
    ui->retail_base->setText(QString::number(base_price_ret, 'f', 2));
    ui->retail_spinbox->setValue(base_price_ret);
    ui->whole_base->setText(QString::number(base_price_whole, 'f', 2));
    ui->whole_spinbox->setValue(base_price_whole);
    ui->begin_spinbox->setValue(old_whole_begin);
}

void WhitePricesEditor::set_name_label(){
    ui->name_label->setText(QString("Наименование: %1").arg(trademark_name));
}

void WhitePricesEditor::apply(){
    if(!base.transaction()){
        QMessageBox::warning(this, "Ошибка базы данных", "Невозможно начать транзакцию.", QMessageBox::Ok);
        return;
    }
    bool changed = false;
    // Проверяем каждое из трёх полей. Если данные изменены - сохраняем их в таблице Trademarks и делаем запись в истории.
    if(ui->retail_spinbox->value() != base_price_ret){
        changed = true;
        if(!save_price(1))
            return;
        base_price_ret = ui->retail_spinbox->value();
    }
    if(ui->whole_spinbox->value() != base_price_whole){
        changed = true;
        if(!save_price(2))
            return;
        base_price_whole = ui->retail_spinbox->value();
    }
    if(ui->begin_spinbox->value() != old_whole_begin){
        changed = true;
        if(!save_price(3))
            return;
        old_whole_begin = ui->begin_spinbox->value();
    }
    base.commit();
    /*if(changed){
        QMessageBox::information(this, "", "Информация обновлена.", QMessageBox::Ok);
        emit accepted();
    }*/
    emit need_reset();
}

void WhitePricesEditor::save(){
    apply();
    QDialog::accept();
}

bool WhitePricesEditor::save_price(int mode){
    QString field = "";
    double value = 0;
    switch(mode){
    case 1:
        field = "price_ret";
        value = ui->retail_spinbox->value();
        break;
    case 2:
        field = "price_whole";
        value = ui->whole_spinbox->value();
        break;
    case 3:
        field = "whole_begin";
        value = ui->begin_spinbox->value();
    }
    QString query_str = QString("UPDATE trademarks SET %1 = '%2' WHERE id = %3").arg(field).arg(value).arg(this->trademark_id);
    QSqlQuery query(base);
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Ошибка базы данных", QString("Не удаётся записать значение.\n%1").arg(query.lastError().text()), QMessageBox::Ok);
        base.rollback();
        return false;
    }

    // делаем запись в истории
    if(!save_history(mode))
        return false;
    return true;
}

bool WhitePricesEditor::save_history(int mode){
    double old_value = 0, new_value = 0;
    switch(mode){
    case 1:
        old_value = base_price_ret;
        new_value = ui->retail_spinbox->value();
        break;
    case 2:
        old_value = base_price_whole;
        new_value = ui->whole_spinbox->value();
        break;
    case 3:
        old_value = old_whole_begin;
        new_value = ui->begin_spinbox->value();
        break;
    }
    QString query_str = QString("INSERT INTO history_prices (trademark_id, datetime, user_id, operation_type, old_value, new_value, reason) "
                                " VALUES (%1, now(), %2, %3, %4, %5, '%6')")
                                .arg(this->trademark_id)
                                .arg(USER_ID)
                                .arg(mode)
                                .arg(old_value)
                                .arg(new_value)
                                .arg(ui->reason_edit->text());
    QSqlQuery query(base);
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Ошибка базы данных", QString("Не удаётся записать в историю.\n%1\n%2").arg(query.lastError().text()).arg(query_str), QMessageBox::Ok);
        base.rollback();
        return false;
    }
    return true;
}

void WhitePricesEditor::enable_percents(bool enabled){
    ui->percents_checkbox->setChecked(enabled);
    ui->plus_button->setEnabled(enabled);
    ui->minus_button->setEnabled(enabled);
    ui->percents_spinbox->setEnabled(enabled);
    if(enabled)
        percents_changed();
}

void WhitePricesEditor::percents_changed(int p){
    short sign = ui->plus_button->isChecked() ? 1 : -1;
    ui->retail_spinbox->setValue(base_price_ret * (1+ui->percents_spinbox->value()/100.00*sign));
    ui->whole_spinbox->setValue(base_price_whole * (1+ui->percents_spinbox->value()/100.00*sign));
}

void WhitePricesEditor::move_up(){
    emit up_clicked();
}

void WhitePricesEditor::move_down(){
    emit down_clicked();
}

void WhitePricesEditor::apply_up(){
    apply();
    move_up();
}

void WhitePricesEditor::apply_down(){
    apply();
    move_down();
}


