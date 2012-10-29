#include "greyadddialog.h"
#include "ui_greyadddialog.h"

GreyAddDialog::GreyAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GreyAddDialog)
{
    ui->setupUi(this);
}

GreyAddDialog::~GreyAddDialog()
{
    delete ui;
}
