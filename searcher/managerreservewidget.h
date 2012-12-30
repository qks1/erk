#ifndef MANAGERRESERVEWIDGET_H
#define MANAGERRESERVEWIDGET_H

#include <QtGui>
#include <QtSql>
#include "whitetable.h"
#include "greytable.h"
#include "common/constants.h"
#include "common/helpers.h"
#include "common/basetableview.h"
#include "docs/movepositions.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// КЛАСС ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// МОДЕЛЬ ТАБЛИЦЫ /////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*
  Отличается от базовой функцией data.
  Ибо цена за единицу и сумма должны преобразовываться к числу с двумя знаками после запятой.
*/

class ReserveProxyModel : public QSortFilterProxyModel{
    Q_OBJECT

public:
    explicit ReserveProxyModel(QWidget *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// КЛАСС ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// ВИДЖЕТ ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

namespace Ui {
class ManagerReserveWidget;
}

class ManagerReserveWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ManagerReserveWidget(ReserveStruct *rstruct,
                                  QWidget *parent = 0);
    ~ManagerReserveWidget();
    void set_grey_table(GreyTable *g);
    void restore_width(int index, int width);
    void restore_order(int logical, int newvisual);
    void set_contragent(int id, QString name);
    void clear_contragent();
    void set_doc_color(int num, bool red = true);

private:
    Ui::ManagerReserveWidget *ui;
    GreyTable *grey_table;
    QSqlQueryModel *source;
    QStandardItemModel *list_model;
    QItemSelectionModel *selection_model;
    ReserveProxyModel *model;
    QMap<QString, QString> columns_names;
    void set_layout();
    void set_columns_names();
    void set_headers();
    void connects();
    void initial_set_contragent();
    bool initial_clear_contragent();

    int max_docs;
    int contragent_id;
    int selected_doctype;
    QSortFilterProxyModel *oldmodel;
    void delpos(int);
    int last_selected_row;
    bool select_all;
    bool all_selected;

    void fill_list();
    int count_selected();

signals:
    void need_refresh();
    void switch_reserves();
    void section_resized(int, int, QString);
    void section_moved(int, int, QString);
    void open_contragents(int, int);
    void clear_contragent_signal();

private slots:
    void change_doc(QModelIndex cur, QModelIndex prev = QModelIndex());
    void add_position();
    void subtr_quant();
    void delete_position();
    void view_doc();
    void restore_doc();
    void clear_doc();
    void move_doc();
    void tobase();
    void create_doc(int, bool);
    void refresh_table();
    double count_sum();
    void open_contragents_slot();
    void text_changed_slot();
    void move_to_doc(int);
    void move_to_reserve(int);
    void selection_changed(QModelIndex i);
    void header_clicked(int);
};



//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// КЛАСС ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// ДОБАВИТЬ ПОЗИЦИЮ ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

class ReserveAddPosition : public QDialog{
    Q_OBJECT

public:
    explicit ReserveAddPosition(int id, int num, int quantity, double retail_price, double whole_price, int whole_begin, QWidget *parent = 0);

private:
    // интерфейс
    QPushButton *accept_button;
    QPushButton *reject_button;
    QLineEdit *quantity_spin;
    QDoubleSpinBox *price;
    QLineEdit *note;
    //QLineEdit *base_price;
    QLineEdit *quantity_total;

    // элементы конструктора
    int grey_id, doc_num;
    int base_quantity;
    double base_retail_price, base_whole_price;
    int base_whole_begin;

    bool price_changed;
    bool editing;

    void set_layout();
    void set_base_price_text(double);


private slots:
    void price_entered();
    void accept();

};


//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// КЛАСС ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// ВЫЧЕСТЬ КОЛИЧЕСТВО ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

class ReserveSubtract : public QDialog{
    Q_OBJECT

public:
    explicit ReserveSubtract(int id, int quantity, QWidget *parent = 0);

private:
    QSpinBox *subtr_spin;
    QLineEdit *quant_line;
    QPushButton *accept_button;
    QPushButton *reject_button;

    int c_id, c_num, c_quantity;

private slots:
    void change_quant(int);
    void accept();

};


//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// КЛАСС ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// ПЕРЕМЕСТИТЬ ПОЗИЦИИ //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// КЛАСС ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// ДИАЛОГ ВЫБОРА ТИПА ДОКУМЕНТА /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

class ReserveSelectDocType : public QDialog{
    Q_OBJECT

public:
    explicit ReserveSelectDocType(QWidget *parent = 0);

private:
    QComboBox *doctypes;
    QCheckBox *nocomplect;
    QPushButton *okbutton;


signals:
    void selected(int, bool);

private slots:
    void ok_clicked();
};


class SmartSplitter : public QSplitter{
    Q_OBJECT

public:
    explicit SmartSplitter(QWidget *parent = 0);

protected:
    void mouseReleaseEvent(QMouseEvent *);

signals:
    void mouse_released();
    void splitter_moved(int, int);

private slots:
    void splitter_moved_slot();
};


#endif // MANAGERRESERVEWIDGET_H
