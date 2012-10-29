#include "whitepriceseditor.h"
#include "ui_whitepriceseditor.h"

WhitePricesEditor::WhitePricesEditor(int id, double price_ret, double price_whole, int whole_begin, QString unit, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WhitePricesEditor)
{
    ui->setupUi(this);
    this->setWindowTitle("Редактирование цен");
    ui->unit_label->setText(unit);
    // сохраняем значения из конструктора
    this->id = id;
    this->old_price_ret = price_ret;
    this->old_price_whole = price_whole;
    this->old_whole_begin = whole_begin;

    fill_values();

    QObject::connect(this->ui->cancel_button, SIGNAL(clicked()), SLOT(reject()));
    QObject::connect(this->ui->apply_button, SIGNAL(clicked()), SLOT(apply()));
    QObject::connect(this->ui->save_button, SIGNAL(clicked()), SLOT(save()));
}

WhitePricesEditor::~WhitePricesEditor()
{
    delete ui;
}


void WhitePricesEditor::fill_values(){
    ui->retail_spinbox->setValue(old_price_ret);
    ui->whole_spinbox->setValue(old_price_whole);
    ui->begin_spinbox->setValue(old_whole_begin);
}

void WhitePricesEditor::apply(){
    if(!base.transaction()){
        QMessageBox::warning(this, "Ошибка базы данных", "Невозможно начать транзакцию.", QMessageBox::Ok);
        return;
    }
    bool changed = false;
    // Проверяем каждое из трёх полей. Если данные изменены - сохраняем их в таблице Trademarks и делаем запись в истории.
    if(ui->retail_spinbox->value() != old_price_ret){
        changed = true;
        if(!save_price(1))
            return;
        old_price_ret = ui->retail_spinbox->value();
    }
    if(ui->whole_spinbox->value() != old_price_whole){
        changed = true;
        if(!save_price(2))
            return;
        old_price_whole = ui->retail_spinbox->value();
    }
    if(ui->begin_spinbox->value() != old_whole_begin){
        changed = true;
        if(!save_price(3))
            return;
        old_whole_begin = ui->begin_spinbox->value();
    }
    base.commit();
    if(changed){
        QMessageBox::information(this, "", "Информация обновлена.", QMessageBox::Ok);
        emit accepted();
    }
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
    QString query_str = QString("UPDATE trademarks SET %1 = '%2' WHERE id = %3").arg(field).arg(value).arg(this->id);
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
        old_value = old_price_ret;
        new_value = ui->retail_spinbox->value();
        break;
    case 2:
        old_value = old_price_whole;
        new_value = ui->whole_spinbox->value();
        break;
    case 3:
        old_value = old_whole_begin;
        new_value = ui->begin_spinbox->value();
        break;
    }
    QString query_str = QString("INSERT INTO history_prices (trademark_id, datetime, user_id, operation_type, old_value, new_value, reason) "
                                " VALUES (%1, now(), %2, %3, %4, %5, '%6')")
                                .arg(this->id)
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
