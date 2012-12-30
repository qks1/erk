#ifndef GREYMOVEDIALOG_H
#define GREYMOVEDIALOG_H

#include <QDialog>
#include <QtSql>
#include "greyadddialog.h"

namespace Ui {
class GreyMoveDialog;
}

class GreyMoveDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GreyMoveDialog(int id,
                            int status,
                            int grey_id,
                            int quantity,
                            QString name,
                            QString year,
                            QString inspection,
                            QString pack,
                            QString storage,
                            QString rack,
                            QString board,
                            QString box,
                            QString category,
                            QString add_par_1,
                            QString add_par_2,
                            QWidget *parent = 0);
    ~GreyMoveDialog();
    
private:
    Ui::GreyMoveDialog *ui;
    int id, status, quantity, grey_id;
    QString storage, rack, board, box, year, inspection, pack, category, addpar1, addpar2;

    void fill_boxes();

    void fill_storages_box();
    void fill_racks_box();
    void fill_boards_box();
    void fill_boxes_box();

signals:
    void need_refresh();

private slots:
    void change_remain(int);
    void apply();


};

#endif // GREYMOVEDIALOG_H
