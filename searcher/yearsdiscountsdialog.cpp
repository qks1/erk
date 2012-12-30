#include "yearsdiscountsdialog.h"
#include "ui_yearsdiscountsdialog.h"

YearsDiscountsDialog::YearsDiscountsDialog(int id, QString name, double retail_price, double whole_price, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::YearsDiscountsDialog)
{
    ui->setupUi(this);
    this->trademark_id = id;
    this->name = name;
    this->retail_baseprice = retail_price;
    this->whole_baseprice = whole_price;
    this->setWindowTitle("Скидки по годам");
    ui->name_label->setText(name);
    ui->price_label->setText(QString("Цена: розн. %1, опт. %2").arg(retail_price, 0,'f',2).arg(whole_price, 0, 'f', 2));
    ui->price_label->adjustSize();
    ui->add_button->setIcon(QIcon(":/plus"));
    ui->edit_button->setIcon(QIcon(":/pen"));
    ui->delete_button->setIcon(QIcon(":/minus"));
    ui->close_button->setIcon(QIcon(":/back"));

    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);

    fill_table();
    ui->table->resizeColumnsToContents();
    ui->table->adjustSize();


    this->adjustSize();
    this->setFixedSize(this->size());
    QObject::connect(ui->close_button, SIGNAL(clicked()), SLOT(close()));
    QObject::connect(ui->add_button, SIGNAL(clicked()), SLOT(add()));
    QObject::connect(ui->edit_button, SIGNAL(clicked()), SLOT(edit()));
    QObject::connect(ui->delete_button, SIGNAL(clicked()), SLOT(del()));
}

YearsDiscountsDialog::~YearsDiscountsDialog()
{
    delete ui;
}

void YearsDiscountsDialog::add(){
    AddYearDiscount *dlg = new AddYearDiscount(trademark_id, retail_baseprice, whole_baseprice, this);
    dlg->exec();
    fill_table();
}

void YearsDiscountsDialog::edit(){
    AddYearDiscount *dlg = new AddYearDiscount(trademark_id,
                                               retail_baseprice,
                                               whole_baseprice,
                                               ui->table->item(ui->table->currentRow(), 0)->data(Qt::UserRole).toInt(),
                                               this);
    dlg->exec();
    fill_table();
}

void YearsDiscountsDialog::del(){
    if(QMessageBox::question(this, "Удалить?", "Удалить эту скидку?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){
        QSqlQuery query(base);
        QString query_str = QString("DELETE FROM years_discounts WHERE id = %1")
                .arg(ui->table->item(ui->table->currentRow(), 0)->data(Qt::UserRole).toInt());
        if(!query.exec(query_str)){
            QMessageBox::warning(this, "Ошибка", QString("Не удалось удалить запись.\n%1").arg(query.lastError().text()));
            return;
        }
    }
    fill_table();
}

void YearsDiscountsDialog::fill_table(){
    //ui->table->clear();
    // выбираем из базы
    QSqlQuery query(base);
    QString query_str = QString("SELECT id, year, percents, manual, "
                                "CASE WHEN manual = true THEN retail_price ELSE (SELECT price_ret FROM trademarks WHERE id = trademark_id)*((100-percents)/100.0) END AS retail_price, "
                                "CASE WHEN manual = true THEN whole_price ELSE (SELECT price_whole FROM trademarks WHERE id = trademark_id)*((100-percents)/100.0) END AS whole_price "
                                "FROM years_discounts WHERE trademark_id = %1 ORDER BY year").arg(trademark_id);
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос.\n%1").arg(query.lastError().text()));
        return;
    }
    QSqlRecord rec = query.record();
    int i = 0;
    ui->table->setRowCount(query.size());
    int prc;
    while(query.next()){
        ui->table->setItem(i, 0, new QTableWidgetItem(query.value(rec.indexOf("year")).toString()));
        prc = query.value(rec.indexOf("percents")).toInt();
        ui->table->setItem(i, 1, new QTableWidgetItem(prc == 0 ? "" : QString("%1\%").arg(prc)));
        ui->table->setItem(i, 2, new QTableWidgetItem(QString("%1 р.").arg(QString::number(query.value(rec.indexOf("retail_price")).toDouble(), 'f', 2))));
        ui->table->setItem(i, 3, new QTableWidgetItem(QString("%1 р.").arg(QString::number(query.value(rec.indexOf("whole_price")).toDouble(), 'f', 2))));
        ui->table->item(i,0)->setData(Qt::UserRole, query.value(rec.indexOf("id")));
        ui->table->item(i,1)->setData(Qt::UserRole, query.value(rec.indexOf("manual")));
        i++;
    }
}
