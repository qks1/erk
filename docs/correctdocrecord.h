#ifndef CORRECTDOCRECORD_H
#define CORRECTDOCRECORD_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QtSql>
#include "common/constants.h"
#include "common/helpers.h"

namespace Ui {
class CorrectDocRecord;
}

class CorrectDocRecord : public QDialog
{
    Q_OBJECT
    
public:
    explicit CorrectDocRecord(int c_record_id,
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
                              double c_discount_price,
                              bool c_is_discount,
                              double c_weight,
                              double c_sum,
                              double c_discount,
                              double c_discountsum,
                              QWidget *parent = 0);
    ~CorrectDocRecord();
    
private:
    Ui::CorrectDocRecord *ui;
    // конструкторрр
    int record_id;
    int trademark_id;
    int quantity;
    double old_price, old_discountprice;
    double old_sum, discount, old_discountsum;
    bool something_changed;

    inline void resize();
    inline void connects();

private slots:
    void something_changed_slot();
    void recount_prices_slot(QString str = QString());
    void accept();

};

#endif // CORRECTDOCRECORD_H
