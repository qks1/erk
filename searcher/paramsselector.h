#ifndef PARAMSSELECTOR_H
#define PARAMSSELECTOR_H

/*
  Выпадающий список с выбором параметров, а над ним - кнопка, сортирующая таблицу по этому параметру.
  Присутствует только в белом экране.
  Количество отображаемых селекторов равно максимальному количеству непустых параметров среди строк в таблице.
  Если выбраны только детали из одной группы - на кнопках написаны названия параметров, если из нескольких - номера.
  Содержит в себе также целое число, хранящее номер переключателя (т.е. номер параметра), передающееся в конструкторе.
*/

#include <QtGui>
#include <QtSql>
#include "constants.h"
#include "helpers.h"
#include "customcombobox.h"

class ParamsSelector : public QWidget
{
    Q_OBJECT
public:
    ParamsSelector(int number, QWidget *parent = 0);
    int number;

    void set_button_label(QString, int);
    void set_button_label(QString);
    void clear_button_label();
    void add_item(QString);
    void add_items(QStringList);
    void clear_items();
    void set_selected(int);
    int count();
    void setEnabled(bool);
    void setModel(QSqlQueryModel*);
    void setModelColumn(int);
    QString item(int);
    void set_edit_text(QString);
    void set_model(QSqlQueryModel*);
    void resize_all();

private:
    QPushButton *button;
    CustomComboBox *items;

    inline void set_layout();
    inline void connect();

signals:
    void item_selected(QString, int);
    void button_clicked(int);
    void fill_box_signal(int);

private slots:
    void select_item(QString);
    void click_button();
    void fill_box_slot();

};

#endif // PARAMSSELECTOR_H
