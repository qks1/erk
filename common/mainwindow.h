#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtSql>
#include "searcher.h"
#include "constants.h"
#include "settingsdialog.h"
#include "customtabwidget.h"
#include "helpers.h"
#include "changepricedatedialog.h"
#include "lists/contragentslist.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();


private:
    QStackedWidget *mwidget;
    CustomTabWidget *search_tabs;
    QSettings *settings;
    QSettings *comp_settings;

    QAction *quit_action;
    QAction *columns_action;
    QAction *settings_action;
    QAction *pricedate_action;
    QAction *showcatalog_action;
    QAction *contragents_list_action;
    QAction *reserves_list_action;

    QLabel *indicator;

    ReserveStruct reserve_struct;
    ColumnsStruct white_columns;
    ColumnsStruct grey_columns;

    void create_search_widget();
    void create_actions();
    void create_menu();
    bool reload_base();
    int open_columns_list(int mode);
    double count_reserve_doc_sum(int num);
    inline ColumnsStruct form_columns_struct(QString);
    inline void save_columns_struct(ColumnsStruct, QString);
    bool prices_access;
    QWidget *doubtful_widget;

    void closeEvent(QCloseEvent *e);
    void keyPressEvent(QKeyEvent *);


signals:
    void exit_signal();


private slots:
    void send_exit();
    void send_settings();
    void send_columns();
    void send_pricedate();
    void check_menu_catalog();
    void uncheck_menu_catalog();
    void send_showcatalog(bool);
    void refresh_white_tables();
    void refresh_grey_tables();
    void add_tab();
    void save_width(int, int, QString);
    void save_order(int, int, QString);
    void switch_indicator();
    void open_contragents_list();
    void close_contragents_list();
    void open_reserves_list();

public slots:


};


#endif // MAINWINDOW_H
