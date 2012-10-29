#ifndef YEARSDISCOUNTSDIALOG_H
#define YEARSDISCOUNTSDIALOG_H

#include <QDialog>
#include "addyeardiscount.h"

namespace Ui {
class YearsDiscountsDialog;
}

class YearsDiscountsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit YearsDiscountsDialog(int id, QString name, double retail_price, double whole_price, QWidget *parent = 0);
    ~YearsDiscountsDialog();
    int trademark_id;
    QString name;
    double retail_baseprice, whole_baseprice;
    
private:
    Ui::YearsDiscountsDialog *ui;
    void fill_table();

private slots:
    void add();
    void edit();
    void del();
};

#endif // YEARSDISCOUNTSDIALOG_H
