#include "mainwindow.h"

MainWindow::MainWindow()
{
    init_vars();
    this->setWindowTitle("ООО \"ЭРК\"");
    search_tabs = 0;
    mwidget = new QStackedWidget();
    settings = get_settings();
    comp_settings = get_comp_settings();
    this->setCentralWidget(mwidget);
    // создаём поисковик
    create_search_widget();

    // создаём действия
    create_actions();

    // создаём меню
    create_menu();
}

MainWindow::~MainWindow(){
    delete mwidget;
    delete settings;
    delete comp_settings;
}

void MainWindow::closeEvent(QCloseEvent *){
    //searcher->close_func();
}

void MainWindow::create_menu(){
    QMenu *menu = menuBar()->addMenu("Меню");
    menu->addAction(columns_action);
    menu->addAction(settings_action);
    menu->addAction(quit_action);

    if(get_privilege(Privileges::Prices_view_access)){
        menu = menuBar()->addMenu("Цены");
        menu->addAction(pricedate_action);
    }

    menu = menuBar()->addMenu("Вид");
    menu->addAction(showcatalog_action);
}

void MainWindow::create_actions(){
    quit_action = new QAction("Выход", this);
    quit_action->setStatusTip("Выход из программы");
    connect(quit_action, SIGNAL(triggered()), this, SLOT(send_exit()));

    settings_action = new QAction("Настройки", this);
    settings_action->setStatusTip("Открыть окно настроек");
    connect(settings_action, SIGNAL(triggered()), this, SLOT(send_settings()));

    columns_action = new QAction("Столбцы", this);
    columns_action->setStatusTip("Выбрать отображаемые столбцы");
    connect(columns_action, SIGNAL(triggered()), this, SLOT(send_columns()));

    pricedate_action = new QAction("Дата изм. цены", this);
    pricedate_action->setStatusTip("Установить дату изменения цены");
    connect(pricedate_action, SIGNAL(triggered()), this, SLOT(send_pricedate()));

    showcatalog_action = new QAction("Показать каталог", this);
    showcatalog_action->setStatusTip("Скрыть/показать каталог");
    showcatalog_action->setCheckable(true);
    showcatalog_action->setChecked(settings->value(QString("%1/show_catalog").arg(USERNAME), true).toBool());
    connect(showcatalog_action, SIGNAL(triggered(bool)), this, SLOT(send_showcatalog(bool)));
}


void MainWindow::send_exit(){
    emit(exit_signal());
}

void MainWindow::send_settings(){
    // открыть диалоговое окно с настройками
    SettingsDialog *sd = new SettingsDialog();
    if(sd->exec() == QDialog::Accepted){
        delete sd;
        settings->beginGroup(QString(USERNAME) + "/DATABASE");
        // если нажата кнопка ОК и хоть что-то изменено, пересоздаём соединение с базой
        if(base.databaseName() != settings->value("basename").toString() ||
           base.hostName() != settings->value("basehost").toString() ||
           base.userName() != settings->value("baseuser").toString() ||
           base.password() != settings->value("basepass").toString()){
            QString old_basename = base.databaseName();
            QString old_host = base.hostName();
            QString old_user = base.userName();
            QString old_pass = base.password();
            settings->endGroup();
            bool t = reload_base();
            settings->beginGroup(QString(USERNAME) + "/DATABASE");
            if(!t){
                settings->setValue("basename", old_basename);
                settings->setValue("basehost", old_host);
                settings->setValue("baseuser", old_user);
                settings->setValue("basepass", old_pass);
                base.setDatabaseName(old_basename);
                base.setUserName(old_user);
                base.setPassword(old_pass);
                base.setHostName(old_host);
                settings->sync();
            }
        }
        settings->endGroup();
        // если измененён шрифт, меняем его
        QFont new_font;
        comp_settings->beginGroup("FONTS");
        new_font.setFamily(comp_settings->value("system_family").toString());
        new_font.setPointSize(comp_settings->value("system_size").toInt());
        if(new_font != qApp->font()){
            QApplication::setFont(new_font, "QWidget");
            if(search_tabs > 0){
                Searcher *s;
                for(int i = 0; i < search_tabs->count(); i++){
                    s = static_cast<Searcher*>(search_tabs->widget(i));
                    s->resize_all();
                }
            }
        }
        comp_settings->endGroup();
        if(settings->value(QString("%1/PHOTOS_PATH").arg(USERNAME)).toString() != PHOTOS_PATH)
            PHOTOS_PATH = settings->value(QString("%1/PHOTOS_PATH").arg(USERNAME)).toString();
        if(comp_settings->value("path").toString() != SETTINGS_PATH){
            SETTINGS_PATH = comp_settings->value("path").toString();
            QMessageBox::information(0, "Внимание", "Чтобы новые настройки вступили в силу, необходимо перезапустить программу");
        }
        return;
    }
    delete sd;
}


void MainWindow::send_columns(){
    if(open_columns_list(GLOBAL_MODE) == QDialog::Accepted){
        // скрываем/показываем нужные столбцы
        if(GLOBAL_MODE == SEARCHER_WHITE_MODE || GLOBAL_MODE == SEARCHER_GREY_MODE){
            Searcher *s;
            for(int i = 0; i < search_tabs->count(); i++){
                s = static_cast<Searcher*>(search_tabs->widget(i));
                s->hide_show_white_columns();
                s->hide_show_grey_columns();
            }
        }
    }
}

void MainWindow::send_pricedate(){
    ChangePriceDateDialog *dlg = new ChangePriceDateDialog(this);
    if(dlg->exec() == QDialog::Accepted){
        Searcher *s;
        for(int i = 0; i < search_tabs->count(); i++){
            s = static_cast<Searcher*>(search_tabs->widget(i));
            s->set_date();
        }
    }
}

void MainWindow::send_showcatalog(bool state){
    Searcher *s;
    for(int i = 0; i < search_tabs->count(); i++){
        s = static_cast<Searcher*>(search_tabs->widget(i));
        state ? s->show_white_catalog() : s->hide_white_catalog();
    }

}

int MainWindow::open_columns_list(int mode){
    if(mode == SEARCHER_WHITE_MODE || mode == SEARCHER_GREY_MODE){
        Searcher *s = static_cast<Searcher*>(search_tabs->currentWidget());
        return (s->mode() == WHITE_MODE ? s->open_white_columns_list() : s->open_grey_columns_list());
    }
    return -1;
}

bool MainWindow::reload_base(){
    // закрываем текущее соединение и создаём новое
    base.close();
    //QSqlDatabase::removeDatabase(base.connectionName());
    //base = QSqlDatabase::addDatabase("QPSQL");
    settings->beginGroup(QString(USERNAME) + "/DATABASE");
    QString DB_NAME = settings->value("basename", "erkbase").toString();
    QString DB_USER = settings->value("baseuser", "postgres").toString();
    QString DB_PASS = settings->value("basepass", "aurora2174_").toString();
    QString DB_HOST = settings->value("basehost", "192.168.1.101").toString();
    settings->endGroup();
    base.setDatabaseName(DB_NAME);
    base.setUserName(DB_USER);
    base.setPassword(DB_PASS);
    base.setHostName(DB_HOST);
    // если base.open завершается неудачно, выдаём ошибку
    if(!(base.open())){
        QMessageBox::warning(0, "Ошибка", "Не удалось подключиться к базе данных. Проверьте настройки.\n" + base.lastError().text(), QMessageBox::Ok);
        is_connect = false;
        return false;
    }
    else{
        if(!is_connect)
            is_connect = true;
        else{
            if(mwidget->indexOf(search_tabs) != -1){
                mwidget->removeWidget(search_tabs);
                delete search_tabs;
            }
        }
        create_search_widget();
    }
    return true;
}

void MainWindow::create_search_widget(){
    // создаём tabWidget с поисковиками
    search_tabs = new CustomTabWidget(mwidget);

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
    for(int i = 0; i < settings->value(QString("%1/tabs_default").arg(USERNAME), 5).toInt(); i++){
        Searcher *s = search_tabs->add_tab();
        if(s->success == false)
            break;
        QObject::connect(s, SIGNAL(open_settings()), this, SLOT(send_settings()));
        QObject::connect(s, SIGNAL(catalog_hides()), this, SLOT(uncheck_menu_catalog()));
        QObject::connect(s, SIGNAL(catalog_shows()), this, SLOT(check_menu_catalog()));
    }

    mwidget->addWidget(search_tabs);
}

void MainWindow::check_menu_catalog(){
    showcatalog_action->setChecked(true);
}

void MainWindow::uncheck_menu_catalog(){
    showcatalog_action->setChecked(false);
}
