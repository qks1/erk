#ifndef GREYADDDIALOG_H
#define GREYADDDIALOG_H

#include <QDialog>
#include <QtSql>
#include "../common/constants.h"
#include "../common/helpers.h"

namespace Ui {
class GreyAddDialog;
}

class GreyAddDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GreyAddDialog(int id, QString name, int status, QWidget *parent = 0);
    GreyAddDialog(int id,
                  QString name,
                  int place_id,
                  int status,
                  QWidget *parent = 0);
    GreyAddDialog(int id,
                  int grey_id,
                  int old_quantity,
                  int category_id,
                  int inspection_id,
                  QString name,
                  QString year,
                  QString pack,
                  QString storage,
                  QString rack,
                  QString board,
                  QString box,
                  QString add_par_1,
                  QString add_par_2,
                  int status,
                  bool moving,
                  QWidget *parent = 0);

    ~GreyAddDialog();
    
private:
    Ui::GreyAddDialog *ui;
    int trademark_id, grey_id, place_id, old_quantity;
    int status;
    QString success_text;
    QMap<QString, QString> params;
    bool editing, moving;

    inline void common_constructor(int id, QString name, int status);
    void connects();

    // заполнение комбобоксов
    void fill_inspections_box();
    void fill_years_box();
    void fill_storages_box();
    void fill_racks_box();
    void fill_boards_box();
    void fill_boxes_box();
    void fill_categories_box();
    void fill_addpar1_box();
    void fill_addpar2_box();

    // найти в базе id места
    int find_place();
    // проверить, есть ли в базе деталь с такими параметрами. Если есть - вернуть id и кол-во
    int detail_exists(int);
    // добавить кол-во, если окажется, что такая деталь есть
    bool add_quantity(int);
    // создать новый id и вычесть кол-во из старого, если редактируется только часть кол-ва
    bool create_new_grey();

signals:
    void need_refresh();
    void set_place_id(int);

private slots:
    void storage_selected(int item = 0);
    void rack_selected(int item = 0);
    void board_selected(int item = 0);
    void box_selected(int item = 0);
    bool save_new(bool transaction = true);
    bool save_edited();
    void fill_current();
    void change_remain(int);

    bool apply();
    void accept();

};

#endif // GREYADDDIALOG_H
