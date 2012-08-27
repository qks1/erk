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

MainWindow::~MainWindow(){
}

void MainWindow::closeEvent(QCloseEvent *){
    //searcher->close_func();
}

QTabWidget* MainWindow::create_search_widget(){
    // создаём tabWidget с поисковиками
    CustomTabWidget *search_tabs = new CustomTabWidget;

    // расположить вкладки снизу
    search_tabs->setTabPosition(QTabWidget::South);

    search_tabs->setTabsClosable(true);

    // создаём и добавляем кнопку добавления таба
    QPushButton *addtab = new QPushButton("+");
    addtab->setFixedSize(20,20);
    QObject::connect(addtab, SIGNAL(clicked()), search_tabs, SLOT(add_tab()));
    search_tabs->setCornerWidget(addtab, Qt::TopRightCorner);

    // на каждый таб создадим свой Searcher
    // кол-во вкладок по умолчанию устанавливается в constants.cpp
    for(int i = 0; i < TABS_DEFAULT; i++){
        search_tabs->add_tab();
    }

    QObject::connect(search_tabs, SIGNAL(tabCloseRequested(int)), search_tabs, SLOT(close_tab(int)));

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

CustomTabWidget::CustomTabWidget(QWidget *parent){
    max = 0;
}

void CustomTabWidget::close_tab(int index){
    if(this->count() <= 1)
        return;
    QWidget *d = widget(index);
    this->removeTab(index);
    delete(d);
}

void CustomTabWidget::add_tab(){
    Searcher *s = new Searcher();
    this->max++;
    this->addTab(s, QString::number(this->max));
}


