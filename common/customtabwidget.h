#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QtGui>
#include "searcher.h"
#include "constants.h"

//class Searcher;

class CustomTabWidget : public QTabWidget{
    Q_OBJECT
public:
    explicit CustomTabWidget(QWidget *parent = 0);
    void switch_hidden();

private:
    int max;

signals:
    void save_width_signal(int, int, QString);
    void save_order_signal(int, int, QString);

public slots:
    void close_tab(int index);
    Searcher *add_tab(ReserveStruct*, ColumnsStruct*, ColumnsStruct*);
    void set_tab_name(QString name, bool save = false);

private slots:
    void save_searcher_width(int mode, int index, int width);
    void save_searcher_order(int mode, int logical, int newvisual);
    void save_width(int index, int width, QString section);
    void save_order(int logical, int newvisual, QString section);
    void switch_reserves();
    void set_reserve_header();
    void refresh_white_searcher();
    void refresh_grey_searcher();
    void refresh_blue_searcher();
    void refresh_searcher();
    void set_reserve_contragent(int id, QString name);
    void clear_reserve_contragent();



signals:
    void open_settings();
};


#endif // CUSTOMTABWIDGET_H
