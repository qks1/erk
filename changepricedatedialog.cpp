#include "changepricedatedialog.h"
#include "ui_changepricedatedialog.h"

ChangePriceDateDialog::ChangePriceDateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePriceDateDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Дата изм. цены");
    set_default_date();

    QObject::connect(ui->ok_button, SIGNAL(clicked()), SLOT(accept()));
    QObject::connect(ui->cancel_button, SIGNAL(clicked()), SLOT(reject()));
}

ChangePriceDateDialog::~ChangePriceDateDialog()
{
    delete ui;
}

void ChangePriceDateDialog::set_default_date(){
    QSettings *settings = get_settings();
    this->ui->dateEdit->setDate(settings->value(QString("%1/price_change_date").arg(USERNAME), QDate::currentDate()).toDate());
    delete settings;
}

void ChangePriceDateDialog::accept(){
    QSettings *settings = get_settings();
    if(settings->value(QString("%1/price_change_date").arg(USERNAME), QDate::currentDate()).toDate() != this->ui->dateEdit->date()){
        settings->setValue(QString("%1/price_change_date").arg(USERNAME), this->ui->dateEdit->date());
    }
    settings->sync();
    delete settings;
    QDialog::accept();
}
