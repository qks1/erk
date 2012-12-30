#ifndef YEARSDISCOUNTSITEM_H
#define YEARSDISCOUNTSITEM_H

#include <QWidget>
#include <QFrame>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QDialog>
#include <QGridLayout>
#include <QEventLoop>
#include <QLabel>
#include <QCheckBox>
#include <QFont>
#include <QtGui>

namespace Ui {
class YearsDiscountsItem;
}

struct two_prices{
    double retail;
    double whole;
};

class YearsDiscountsItem : public QFrame
{
    Q_OBJECT
    
public:
    explicit YearsDiscountsItem(double retail, double whole, QWidget *parent = 0);

    void set_year(QString year);
    void set_percents(int percents);
    void set_retail(double price);
    void set_whole(double price);
    void set_manual(bool manual);
    void set_plusminus(bool mode);
    // и наоборот
    QString year();
    int percents();
    double retail_price();
    double whole_price();
    bool manual();
    bool mode();
    void set_base_retail(double price);
    void set_base_whole(double price);
    void set_retail_enabled(bool);
    void set_whole_enabled(bool);
    void set_retail_mode(bool);
    void set_whole_mode(bool);
    // coordinates
    void set_formed();
    void set_unformed();
    int year_label_x();
    int retail_label_x();
    int whole_label_x();
    int percents_spinbox_x();
    int manual_checkbox_x();



    
private:
    bool change_retail, change_whole, formed;
    double base_retail, base_whole;
    bool discount_mode;

    QLabel *year_label, *retail_label, *whole_label, *retail_rub, *whole_rub;
    QPushButton *plusminus_button;
    QSpinBox *percents_spinbox;
    QCheckBox *manual_checkbox;
    double dscnt_round(double base);

private slots:
    void change_mode();
    void switch_manual(bool);
    void manual_edit();

public slots:
    void count_prices();
    void count_retail();
    void count_whole();

};

//-------------------------------------------------------------

class ManualPricesDialog : public QDialog{
    Q_OBJECT

public:
    explicit ManualPricesDialog(double retail, double whole, QWidget *parent = 0);
    two_prices exec();

private:
    double old_retail, old_whole;
    QEventLoop event_loop;
    two_prices prices;
    QDoubleSpinBox *retail;
    QDoubleSpinBox *whole;
    QPushButton *accept_button;
    QPushButton *reject_button;

private slots:
    void accept();
    void reject();

};

#endif // YEARSDISCOUNTSITEM_H
