#include "correctdocrecord.h"
#include "ui_correctdocrecord.h"

CorrectDocRecord::CorrectDocRecord(int c_record_id,
                                   int c_trademark_id,
                                   QString c_trademark,
                                   QString c_print_line,
                                   QString c_storage,
                                   QString c_rack,
                                   QString c_board,
                                   QString c_box,
                                   QString c_year,
                                   QString c_notes,
                                   int c_quantity,
                                   double c_price,
                                   double c_discountprice,
                                   bool c_is_discount,
                                   double c_weight,
                                   double c_sum,
                                   double c_discount,
                                   double c_discountsum,
                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CorrectDocRecord)
{
    ui->setupUi(this);
    QFont font = qApp->font();
    font.setPointSize(font.pointSize() - 1);
    this->setFont(font);
    this->setWindowTitle("Коррекция записи");
    this->record_id = c_record_id;
    this->trademark_id = c_trademark_id;
    this->quantity = c_quantity;
    this->old_price = c_price;
    this->old_discountprice = c_discountprice;
    this->old_sum = c_sum;
    this->discount = c_discount;
    this->old_discountsum = c_discountsum;

    // заполняем чо надо
    ui->trademark_line->setText(c_trademark);
    ui->print_line->setText(c_print_line);
    ui->storage_line->setText(c_storage);
    ui->rack_line->setText(c_rack);
    ui->board_line->setText(c_board);
    ui->box_line->setText(c_box);
    ui->year_line->setText(c_year);
    ui->notesedit->setPlainText(c_notes);
    ui->price_line->setText(QString::number(c_price, 'f', 2));
    ui->discount_checkbox->setChecked(c_is_discount);
    if(c_weight > 0) ui->weight_line->setText(QString::number(c_weight, 'f', 2));
    ui->weight_checkbox->setChecked(false);
    ui->sum_label->setText(QString::number(c_sum, 'f', 2));
    ui->discount_label->setText(QString::number(c_discount, 'f', 2));
    ui->discountsum_label->setText(QString::number(c_discountsum, 'f', 2));

    QDoubleValidator *price_val = new QDoubleValidator(ui->price_line);
    ui->price_line->setValidator(price_val);
    QDoubleValidator *weight_val = new QDoubleValidator(ui->weight_line);
    ui->weight_line->setValidator(weight_val);

    something_changed = false;

    resize();
    connects();
}

CorrectDocRecord::~CorrectDocRecord()
{
    delete ui;
}

inline void CorrectDocRecord::resize(){
    int f = qApp->font().pointSize();
    ui->storage_box->setFixedHeight(f*8);

    ui->sum_box->setFixedWidth(f*12);
    ui->discount_box->setFixedWidth(ui->sum_box->width());
    ui->discountsum_box->setFixedWidth(ui->sum_box->width());
    ui->sum_label->setFixedWidth(f*9);
    ui->discountsum_label->setFixedWidth(f*9);
}

inline void CorrectDocRecord::connects(){
    connect(ui->ok_button, SIGNAL(clicked()), SLOT(accept()));
    connect(ui->cancel_button, SIGNAL(clicked()), SLOT(reject()));
    connect(ui->print_line, SIGNAL(textChanged(QString)), SLOT(something_changed_slot()));
    connect(ui->notesedit, SIGNAL(textChanged()), SLOT(something_changed_slot()));
    connect(ui->price_line, SIGNAL(textChanged(QString)), SLOT(something_changed_slot()));
    connect(ui->discount_checkbox, SIGNAL(clicked()), SLOT(something_changed_slot()));
    connect(ui->weight_line, SIGNAL(textChanged(QString)), SLOT(something_changed_slot()));
    connect(ui->price_line, SIGNAL(textChanged(QString)), SLOT(recount_prices_slot(QString)));
    connect(ui->discount_checkbox, SIGNAL(clicked()), SLOT(recount_prices_slot()));
}

void CorrectDocRecord::something_changed_slot(){
    something_changed = true;
}

void CorrectDocRecord::recount_prices_slot(QString str){
    if(str == QString()) str = ui->price_line->text();
    double newprice = str.toDouble();
    if(str < 0)
        return;
    // цена без скидки (sum_label): берём старую сумму,
    // отнимаем (старая цена*кол-во), прибавляем (новая цена*кол-во)
    ui->sum_label->setText(QString::number(old_sum-(old_price-newprice)*quantity));
    // цена со скидкой (discountsum_label): берём старую сумму со скидкой,
    // если скидки нет - алгоритм аналогичен предыдущему
    // если есть - считаем новую цену со скидкой и далее аналогично предыдущему
    if(ui->discount_checkbox->isChecked())
        newprice *= (1.00-discount/100);
    ui->discountsum_label->setText(QString::number(old_discountsum-(old_discountprice-newprice)*quantity));
    ui->sum_label->adjustSize();
    ui->discountsum_label->adjustSize();

}

void CorrectDocRecord::accept(){
    // проверяем, корректно ли введены цена и вес
    double newprice = ui->price_line->text().toDouble();
    if(newprice <= 0.0){
        if(QMessageBox::question(this, "Цена", "Не введена положительная цена! Записать нулевую цену?", "Да", "Нет") == 1)
            return;
        else newprice = 0;
    }
    double newweight = -1;
    if(ui->weight_line->text().length() > 0){
        newweight = ui->weight_line->text().toDouble();
        if(newweight <= 0.0){
            QMessageBox::warning(this, "Вес", "Не введён положительный вес!", "OK");
            return;
        }
    }

    /* записываем:
      ui->print_line (print_string)
      ui->notesedit (notes)
      ui->price_edit (price)
      ui->discount_checkbox (is_discount)
      ui->weight_line (weight)
      discount_price (если !discount, то price, иначе price*(1-discount)
      trademarks.weight, если weight_checkbox
    */
    QSqlQuery query(base);
    QString query_str;
    if(!base.transaction()){
        QMessageBox::warning(this, "Ошибка", "Не удаётся начать транзакцию. " + base.lastError().text() + " (accept)", QMessageBox::Ok);
        return;
    }
    query_str = QString("UPDATE docs_details SET print_string = '%1', "
                        " notes = '%2', price = %3, is_discount = %4, "
                        " discount_price = %5")
            .arg(ui->print_line->text())
            .arg(ui->notesedit->toPlainText())
            .arg(newprice)
            .arg(ui->discount_checkbox->isChecked() ? "true" : "false")
            .arg(ui->discount_checkbox->isChecked() ? newprice*(1.00-discount/100) : newprice);
    if(newweight > 0)
        query_str.append(QString(", weight = %1").arg(newweight));
    query_str.append(QString(" WHERE id = %1").arg(record_id));
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Ошибка", QString("Невозможно выполнить запрос: %1 (accept_1)").arg(query.lastError().text()), QMessageBox::Ok);
        base.rollback();
        return;
    }

    if(ui->weight_checkbox->isChecked() && newweight > 0){
        query_str = QString("UPDATE trademarks SET weight = %1 WHERE id = %2").arg(newweight).arg(trademark_id);
        if(!query.exec(query_str)){
            QMessageBox::warning(this, "Ошибка", QString("Невозможно выполнить запрос: %1 (accept_2)").arg(query.lastError().text()), QMessageBox::Ok);
            base.rollback();
            return;
        }
    }

    base.commit();
    something_changed = false;
    QDialog::accept();
}
