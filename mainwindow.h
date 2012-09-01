#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtSql>
#include "searcher.h"
#include "constants.h"
#include "settingsdialog.h"

class CustomTabWidget : public QTabWidget{
    Q_OBJECT
public:
    explicit CustomTabWidget(QWidget *parent = 0);

private:
    int max;

public slots:
    void close_tab(int index);
    void add_tab();

signals:
    void resize_section(int, int, int);
    void move_section(int);
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(bool);


private:
    bool is_connect;
    QStackedWidget *mwidget;
    CustomTabWidget *search_tabs;
    QSettings *settings;

    QAction *quit_action;
    QAction *settings_action;
    QAction *columns_action;

    void create_search_widget();
    void create_actions();
    void create_menu();
    bool reload_base();
    void open_columns_list(int mode);
    void save_searcher_width(int mode, int index, int width);
    void save_searcher_order(int mode);

    void closeEvent(QCloseEvent *);


signals:
    void exit_signal();


private slots:
    void send_exit();
    void send_settings();
    void send_columns();
    void save_width(int mode, int index, int width);
    void save_order(int mode);

public slots:


};

#endif // MAINWINDOW_H
