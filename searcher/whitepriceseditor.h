#ifndef WHITEPRICESEDITOR_H
#define WHITEPRICESEDITOR_H

#include <QDialog>
#include "constants.h"
#include "helpers.h"

namespace Ui {
class WhitePricesEditor;
}

class WhitePricesEditor : public QDialog
{
    Q_OBJECT
    
public:
    /* в конструкторе мы передаём следующие параметры:
         - id;
         - розничная цена;
         - оптовая цена;
         - со скольких начинается опт;
    */
    explicit WhitePricesEditor(int id, QString name, double price_ret, double price_whole, int whole_begin, QWidget *parent = 0);
    ~WhitePricesEditor();
    void renew(int id, QString name, double price_ret, double price_whole, int whole_begin);
    
private:
    Ui::WhitePricesEditor *ui;
    // сохранённые значения из конструктора
    int trademark_id;
    double base_price_ret, base_price_whole;
    int old_whole_begin;
    QString trademark_name;
    void fill_values();
    void set_name_label();
    bool save_price(int mode);
    bool save_history(int mode);

signals:
    void up_clicked();
    void down_clicked();
    void need_reset();
    void refresh_searcher();

private slots:
    void apply();
    void save();
    void enable_percents(bool);
    void percents_changed(int p = 0);
    void apply_up();
    void apply_down();
    void move_up();
    void move_down();

};

#endif // WHITEPRICESEDITOR_H
