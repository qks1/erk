#ifndef DETAILCARD_H
#define DETAILCARD_H

#include <QtGui>
#include <QtSql>
#include "helpers.h"
#include "constants.h"
#include "mytablemodel.h"

class DetailCard : public QWidget
{
    Q_OBJECT
public:
    explicit DetailCard(int id, QWidget *parent = 0);
    ~DetailCard();
    int exec();

private:
    int id;
    QWidget *parent;
    void save_state();
    void restore_state();
    void load_default_picture(QPixmap);
    QSettings *settings;
    QTableView *table;
    QStringList column_names;
    

signals:
    
private slots:
    void column_width_changed(int,int,int);
    void column_moved(int,int,int);
    
};

class CardDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CardDialog(QWidget *parent = 0);
    ~CardDialog();

private:
    QSettings *settings;
    void closeEvent(QCloseEvent *);
};

#endif // DETAILCARD_H
