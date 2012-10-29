#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QtGui>
#include "searcher.h"

//class Searcher;

class CustomTabWidget : public QTabWidget{
    Q_OBJECT
public:
    explicit CustomTabWidget(QWidget *parent = 0);

private:
    int max;

public slots:
    void close_tab(int index);
    Searcher *add_tab();

private slots:
    void save_searcher_width(int mode, int index, int width);
    void save_searcher_order(int mode);
    void save_width(int mode, int index, int width);
    void save_order(int mode);



signals:
    void resize_section(int, int, int);
    void move_section(int);
    void open_settings();
};


#endif // CUSTOMTABWIDGET_H
