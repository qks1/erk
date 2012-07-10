#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "searcher.h"
#include "constants.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();


private:
    QTabWidget *search_tabs;
    QAction *quit_action;

    QTabWidget* create_search_widget();
    void create_actions();
    void create_menu();


signals:
    void exit_signal();


private slots:
    void send_exit();

public slots:


};

#endif // MAINWINDOW_H
