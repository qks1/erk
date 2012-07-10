#include "mainwindow.h"

MainWindow::MainWindow()
{
    // создаём виджет поисковика
    search_tabs = create_search_widget();

    // создаём действия
    create_actions();

    // создаём меню
    create_menu();

    // устанавливаем созданный tabWidget в качестве центрального
    this->setCentralWidget(search_tabs);
}

QTabWidget* MainWindow::create_search_widget(){
    // создаём tabWidget с поисковиками
    QTabWidget *search_tabs = new QTabWidget;

    // расположить вкладки снизу
    search_tabs->setTabPosition(QTabWidget::South);

    // на каждый таб создадим свой Searcher
    // кол-во вкладок по умолчанию устанавливается в constants.cpp
    Searcher *searcher = 0;
    for(int i = 0; i < TABS_DEFAULT; i++){
        searcher = new Searcher();
        if(!searcher->ok)
            // если возникли проблемы с созданием поисковика, прерываем процесс
            break;
        search_tabs->addTab(searcher, "");
    }

    return search_tabs;
}

void MainWindow::create_menu(){
    QMenu *menu = menuBar()->addMenu("Меню");
    menu->addAction(quit_action);
}

void MainWindow::create_actions(){
    quit_action = new QAction("Выход", this);
    quit_action->setStatusTip("Выход из программы");
    connect(quit_action, SIGNAL(activated()), this, SLOT(send_exit()));
}


void MainWindow::send_exit(){
    emit(exit_signal());
}


