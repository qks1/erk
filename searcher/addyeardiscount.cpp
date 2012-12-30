#include "addyeardiscount.h"
#include "ui_addyeardiscount.h"

// конструктор, вызываемый при добавлении новой скидки
AddYearDiscount::AddYearDiscount(int trademark_id, double retail_baseprice, double whole_baseprice, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddYearDiscount)
{
    ui->setupUi(this);
    this->setWindowTitle("Добавить");

    // переменная, определяющая режим (добавление или редактирование)
    editing = false;

    // запоминаем id (чтобы добавить в запись БД) и базовые цены (чтобы считать изменённые)
    this->trademark_id = trademark_id;
    this->retail_baseprice = retail_baseprice;
    this->whole_baseprice = whole_baseprice;

    // заполняем комбобокс с годами (1960-текущий, 196*-200*, 19__, 20__)
    fill_years_box();

    // в зависимости от режима (ручной или проценты) активируются спинбоксы с ценами
    // и деактивируется спинбокс с процентами, либо наоборот
    ui->retail_spinbox->setEnabled(false);
    ui->whole_spinbox->setEnabled(false);
    ui->retail_spinbox->setValue(retail_baseprice);
    ui->whole_spinbox->setValue(whole_baseprice);

    connects();

    this->adjustSize();
    this->setFixedSize(this->size());
}

// конструктор для редактирования скидки
// дополнительно передаём id записи в таблице скидок, чтобы подставить в форму значения
AddYearDiscount::AddYearDiscount(int trademark_id, double retail_baseprice, double whole_baseprice, int record_id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddYearDiscount)
{
    ui->setupUi(this);
    this->setWindowTitle("Изменить");

    editing = true;
    this->trademark_id = trademark_id;
    this->retail_baseprice = retail_baseprice;
    this->whole_baseprice = whole_baseprice;
    this->record_id = record_id;
    fill_years_box();

    // составляем sql-запрос для извлечения всех остальных данных
    QSqlQuery query(base);
    if(!query.exec(QString("SELECT * FROM years_discounts WHERE id=%1").arg(record_id))){
        QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос:%1").arg(query.lastError().text()));
        return;
    }
    QSqlRecord rec = query.record();
    query.next();

    // запоминаем значения в атрибуты класса
    this->year = query.value(rec.indexOf("year")).toString();
    this->percents = query.value(rec.indexOf("percents")).toInt();
    this->retail_price = query.value(rec.indexOf("retail_price")).toDouble();
    this->whole_price = query.value(rec.indexOf("whole_price")).toDouble();
    this->manual = query.value(rec.indexOf("manual")).toBool();

    // это конструктор для редактирования, стало быть, подставляем значения
    qDebug() << ui->years_combobox->findText(year);
    ui->years_combobox->setCurrentIndex(ui->years_combobox->findText(year));
    ui->retail_spinbox->setValue(retail_price);
    ui->whole_spinbox->setValue(whole_price);
    ui->percents_spinbox->setValue(percents);
    ui->manual_checkbox->setChecked(manual);

    // в зависимости от значения manual, деактивируем спинбоксы с процентами либо с ценами
    ui->percents_spinbox->setEnabled(!manual);
    ui->retail_spinbox->setEnabled(manual);
    ui->whole_spinbox->setEnabled(manual);

    connects();

    this->adjustSize();
    this->setFixedSize(this->size());

}

AddYearDiscount::~AddYearDiscount()
{
    delete ui;
}

void AddYearDiscount::connects(){
    QObject::connect(this->ui->percents_spinbox, SIGNAL(valueChanged(int)), SLOT(change_price_value(int)));
    QObject::connect(this->ui->ok_button, SIGNAL(clicked()), SLOT(accept()));
    QObject::connect(this->ui->cancel_button, SIGNAL(clicked()), SLOT(reject()));
    QObject::connect(this->ui->manual_checkbox, SIGNAL(clicked(bool)), SLOT(manual_checked(bool)));
}

void AddYearDiscount::fill_years_box(){
    this->ui->years_combobox->addItem("196*");
    this->ui->years_combobox->addItem("197*");
    this->ui->years_combobox->addItem("198*");
    this->ui->years_combobox->addItem("199*");
    this->ui->years_combobox->addItem("200*");
    this->ui->years_combobox->addItem("19__");
    this->ui->years_combobox->addItem("20__");
    for(int y=1960; y <= QDate::currentDate().year(); y++)
        this->ui->years_combobox->addItem(QString::number(y));
}

void AddYearDiscount::change_price_value(int percents){
    // при изменении процентов меняем розничную и оптовую цены. Пока без округления.
    this->ui->retail_spinbox->setValue(retail_baseprice * (1 - (percents)/100.0));
    this->ui->whole_spinbox->setValue(whole_baseprice * (1 - (percents)/100.0));
}

void AddYearDiscount::manual_checked(bool check){
    // при клике на чекбоксе "вручную" (де)актвируем (не)соответствующие спинбоксы
    if(check){
        ui->percents_spinbox->setEnabled(false);
        ui->retail_spinbox->setEnabled(true);
        ui->whole_spinbox->setEnabled(true);
    }
    else{
        // а если ручной режим отключается - ещё и смотрим на проценты и изменяем значения в спинбоксах с ценами
        ui->percents_spinbox->setEnabled(true);
        ui->retail_spinbox->setEnabled(false);
        ui->whole_spinbox->setEnabled(false);
        ui->retail_spinbox->setValue(retail_baseprice * (1 - ((ui->percents_spinbox->value()))/100.0));
        ui->whole_spinbox->setValue(whole_baseprice * (1 - ((ui->percents_spinbox->value()))/100.0));
    }
}

void AddYearDiscount::accept(){
    // самая ответственная и запутанная часть
    // это просто переменная, определяющая, нужно ли нам в итоге формировать запрос и делать что-то в базе,
    // либо же просто закрыть окно
    bool cont = true;

    QSqlQuery query(base);
    QString query_str;
    // если по этому году уже есть скидка, предлагаем заменить
    query_str = QString("SELECT * FROM years_discounts WHERE trademark_id = %1 AND year = '%2'").arg(trademark_id).arg(ui->years_combobox->currentText());
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос.\n%1").arg(query.lastError().text()));
        return;
    }
    if(query.size() >= 1 || editing){
        // если введены нули (т.е. цены приведены к базовым), предлагаем удалить
        if(ui->retail_spinbox->value() == retail_baseprice && ui->whole_spinbox->value() == whole_baseprice){
            if(QMessageBox::question(this, "Удалить?", "Удалить скидку?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){
                QString query_str = QString("DELETE FROM years_discounts WHERE trademark_id = %1 AND year = '%2'").arg(trademark_id).arg(ui->years_combobox->currentText());
                if(!query.exec(query_str)){
                    QMessageBox::warning(this, "Ошибка", QString("Не удалось удалить скидку.\n%1").arg(query.lastError().text()));
                    return;
                }
                QDialog::accept();
            }
            else cont = false;
        }
        // если цены к нулю таки не приведены, скидку надо заменить
        else{
            // если это не режим редактирования, то спросить об этом.
            if(!editing){
                if(QMessageBox::question(this, "Заменить?", "Для этой детали уже существует скидка на этот год. Заменить?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
                    cont = false;
            }
            if(cont)
                query_str = editing ?
                            QString("UPDATE years_discounts SET year = '%1', percents = %2, retail_price = %3, whole_price = %4, manual = %5, user_id = %6 "
                                    "WHERE trademark_id = %7 AND year = '%8'")
                        .arg(ui->years_combobox->currentText())
                        .arg(ui->manual_checkbox->isChecked() ? 0 : ui->percents_spinbox->value())
                        .arg(QString::number(ui->retail_spinbox->value(), 'f', 2))
                        .arg(QString::number(ui->whole_spinbox->value(), 'f', 2))
                        .arg(ui->manual_checkbox->isChecked() ? "true" : "false")
                        .arg(USER_ID)
                        .arg(trademark_id)
                        .arg(year)
                          :
                QString("UPDATE years_discounts SET percents = %1, retail_price = %2, whole_price = %3, manual = %4, user_id = %5 "
                                    "WHERE trademark_id = %6 AND year = '%7'")
                        .arg(ui->manual_checkbox->isChecked() ? 0 : ui->percents_spinbox->value())
                        .arg(QString::number(ui->retail_spinbox->value(), 'f', 2))
                        .arg(QString::number(ui->whole_spinbox->value(), 'f', 2))
                        .arg(ui->manual_checkbox->isChecked() ? "true" : "false")
                        .arg(USER_ID)
                        .arg(trademark_id)
                        .arg(ui->years_combobox->currentText());

        }
    }
    else if(!(ui->retail_spinbox->value() == retail_baseprice && ui->whole_spinbox->value() == whole_baseprice && !(ui->percents_spinbox->value() > 0))){
        query_str = QString("INSERT INTO years_discounts (trademark_id, year, percents, retail_price, whole_price, manual, user_id)"
                            "VALUES (%1, '%2', '%3', %4, %5, %6, %7)")
                .arg(trademark_id)
                .arg(ui->years_combobox->currentText())
                .arg(ui->manual_checkbox->isChecked() ? 0 : ui->percents_spinbox->value())
                .arg(QString::number(ui->retail_spinbox->value(), 'f', 2))
                .arg(QString::number(ui->whole_spinbox->value(), 'f', 2))
                .arg(ui->manual_checkbox->isChecked() ? "true" : "false")
                .arg(USER_ID);
    }
    else{
        QMessageBox::information(this, "", "Вы не ввели скидку", QMessageBox::Ok);
        cont = false;
    }

    if(cont){
        qDebug() <<  query_str;
        if(!query.exec(query_str)){
            QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос.\n%1").arg(query.lastError().text()));
            return;
        }
        QDialog::accept();
    }
}
