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
    ~MainWindow();


private:
    QTabWidget *search_tabs;
    Searcher *searcher;
    QAction *quit_action;

    QTabWidget* create_search_widget();
    void create_actions();
    void create_menu();

    void closeEvent(QCloseEvent *);


signals:
    void exit_signal();


private slots:
    void send_exit();

public slots:


};

class CustomTabWidget : public QTabWidget{
    Q_OBJECT
public:
    explicit CustomTabWidget(QWidget *parent = 0);

private:
    int max;

public slots:
    void close_tab(int index);
    void add_tab();
};

#endif // MAINWINDOW_H
