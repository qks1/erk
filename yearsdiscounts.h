#ifndef YEARSDISCOUNTS_H
#define YEARSDISCOUNTS_H

#include <QDialog>
#include <QDate>
#include <QLayout>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QList>
#include <QSpacerItem>
#include <QtSql>
#include <QMessageBox>
#include <QDebug>
#include <QtGui>
#include "math.h"
#include "constants.h"
#include "helpers.h"
#include "yearsdiscountsitem.h"

struct discount{
    int id;
    int percents;
    double retail_price;
    double whole_price;
    bool manual;
};

class YearsDiscounts : public QDialog
{
    Q_OBJECT
    
public:
    explicit YearsDiscounts(int id, QString name, double retail, double whole, QWidget *parent = 0);
    ~YearsDiscounts();

    void renew(int id, QString name, double retail, double whole);
    
private:
    int trademark_id;
    double retail_price, whole_price;
    bool filled;

    // видгеты
    QLabel *name_label;
    QList<YearsDiscountsItem*> years_items;
    QCheckBox *remember_checkbox;
    QPushButton *up_button;
    QPushButton *down_button;
    QPushButton *apply_button;
    QPushButton *close_button;
    QCheckBox *apply_retail_checkbox;
    QCheckBox *apply_whole_checkbox;

    // тут будет список годов, на основе коего будет формироваться грид и years_items
    QStringList years_list;

    void set_layout();
    void add_header(int column, QGridLayout *years_layout);

    void form_years_list();
    void form_grid(QGridLayout*);
    void fill_grid();

signals:
    void up_clicked();
    void down_clicked();

private slots:
    void accept();
    void enable_retail(bool);
    void enable_whole(bool);
};

#endif // YEARSDISCOUNTS_H
