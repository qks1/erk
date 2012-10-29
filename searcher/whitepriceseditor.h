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
    explicit WhitePricesEditor(int id, double price_ret, double price_whole, int whole_begin, QString unit, QWidget *parent = 0);
    ~WhitePricesEditor();
    
private:
    Ui::WhitePricesEditor *ui;
    // сохранённые значения из конструктора
    int id;
    double old_price_ret, old_price_whole;
    int old_whole_begin;
    void fill_values();
    bool save_price(int mode);
    bool save_history(int mode);

private slots:
    void apply();
    void save();

};

#endif // WHITEPRICESEDITOR_H
