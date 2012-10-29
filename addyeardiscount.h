#ifndef ADDYEARDISCOUNT_H
#define ADDYEARDISCOUNT_H

#include <QDialog>
#include <QtSql>
#include <QDate>
#include "constants.h"
#include "helpers.h"

namespace Ui {
class AddYearDiscount;
}

class AddYearDiscount : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddYearDiscount(int trademark_id, double retail_baseprice, double whole_baseprice, QWidget *parent = 0);
    AddYearDiscount(int trademark_id, double retail_baseprice, double whole_baseprice, int record_id, QWidget *parent = 0);
    ~AddYearDiscount();
    
private:
    Ui::AddYearDiscount *ui;
    void fill_years_box();
    int trademark_id;
    double retail_baseprice, whole_baseprice;
    int record_id, percents;
    double retail_price, whole_price;
    bool manual;
    QString year;
    bool editing;

    inline void connects();

private slots:
    void change_price_value(int);
    void manual_checked(bool);
    void accept();
};

#endif // ADDYEARDISCOUNT_H
