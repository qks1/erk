#include "mainwindow.h"

MainWindow::MainWindow(bool success)
{
    is_connect = success;
    search_tabs = 0;
    mwidget = new QStackedWidget();
    settings = new QSettings("erk", "base");
    this->setCentralWidget(mwidget);
    // если соединение с базой было успешным, создаём центральный виджет (таблицу с документами).
    // Иначе экран будет пустым, пока не соединимся с базой
    if(is_connect)
        create_search_widget();

    // создаём действия
    create_actions();

    // создаём меню
    create_menu();
}

void MainWindow::closeEvent(QCloseEvent *){
    //searcher->close_func();
}

void MainWindow::create_search_widget(){
    // создаём tabWidget с поисковиками
    search_tabs = new CustomTabWidget();

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

    mwidget->addWidget(search_tabs);

    QObject::connect(search_tabs, SIGNAL(tabCloseRequested(int)), search_tabs, SLOT(close_tab(int)));
    QObject::connect(search_tabs, SIGNAL(resize_section(int,int,int)), SLOT(save_width(int,int,int)));
    QObject::connect(search_tabs, SIGNAL(move_section(int)), SLOT(save_order(int)));
}

void MainWindow::create_menu(){
    QMenu *menu = menuBar()->addMenu("Меню");
    menu->addAction(columns_action);
    menu->addAction(settings_action);
    menu->addAction(quit_action);
}

void MainWindow::create_actions(){
    quit_action = new QAction("Выход", this);
    quit_action->setStatusTip("Выход из программы");
    connect(quit_action, SIGNAL(activated()), this, SLOT(send_exit()));

    settings_action = new QAction("Настройки", this);
    quit_action->setStatusTip("Открыть окно настроек");
    connect(settings_action, SIGNAL(activated()), this, SLOT(send_settings()));

    columns_action = new QAction("Столбцы", this);
    columns_action->setStatusTip("Выбрать отображаемые столбцы");
    connect(columns_action, SIGNAL(activated()), this, SLOT(send_columns()));
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
        settings->beginGroup(QString(USERNAME) + "/FONTS");
        new_font.setFamily(settings->value("system_family").toString());
        new_font.setPointSize(settings->value("system_size").toInt());
        if(new_font != qApp->font()){
            QApplication::setFont(new_font, "QWidget");
            //if(docs_list > 0) docs_list->resize();
            //if(doc > 0) doc->resize();
            //if(preview > 0) preview->resize();
        }
        settings->endGroup();
        if(settings->value(QString("%1/PHOTOS_PATH").arg(USERNAME)).toString() != PHOTOS_PATH)
            PHOTOS_PATH = settings->value(QString("%1/PHOTOS_PATH").arg(USERNAME)).toString();
        return;
    }
    delete sd;
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

void MainWindow::send_columns(){
    open_columns_list(GLOBAL_MODE);
}

void MainWindow::open_columns_list(int mode){
    QDialog *cl = new QDialog(this);
    QListWidget *list = new QListWidget();
    //QListWidgetItem *i1 = new QListWidgetItem("qqqq", list);
    QString c;
    foreach(c, ALL_WHITE_COLUMNS){
        QListWidgetItem *wi = new QListWidgetItem(WHITE_COLUMNS_NAMES[c], list);
        wi->setCheckState(Qt::Checked);
    }

    QHBoxLayout *lt = new QHBoxLayout();
    lt->addWidget(list);
    cl->setLayout(lt);

    cl->exec();
}

void MainWindow::save_searcher_width(int mode, int index, int width){
    QString section;
    if(mode == SEARCHER_WHITE_MODE) section = "WHITE_COLUMNS";
    else if(mode == SEARCHER_GREY_MODE) section = "GREY_COLUMNS";
    // сначала сохраняем настройки
    settings->beginGroup(QString("%1/%2").arg(USERNAME).arg(section));
    settings->setValue(QString("width[%1]").arg(index), width);
    settings->endGroup();
    settings->sync();
    // теперь применяем их ко всем вкладкам
    Searcher *s;
    for(int i = 0; i < this->search_tabs->count(); i++){
        s = static_cast<Searcher*>(search_tabs->widget(i));
        if(mode == SEARCHER_WHITE_MODE) s->restore_white_width(index, width);
        else if(mode == SEARCHER_GREY_MODE) s->restore_grey_width(index, width);
    }
}

void MainWindow::save_searcher_order(int mode){
    Searcher *s;
    for(int i = 0; i < this->search_tabs->count(); i++){
        s = static_cast<Searcher*>(search_tabs->widget(i));
        if(mode == SEARCHER_WHITE_MODE) s->restore_white_order();
        else if(mode == SEARCHER_GREY_MODE) s->restore_grey_order();
    }
}

void MainWindow::save_width(int mode, int index, int width){
    switch(mode){
    case SEARCHER_WHITE_MODE:
        save_searcher_width(mode, index, width);
        break;
    case SEARCHER_GREY_MODE:
        save_searcher_width(mode, index, width);
        break;
    }
}

void MainWindow::save_order(int mode){
    switch(mode){
    case SEARCHER_WHITE_MODE:
        save_searcher_order(mode);
        break;
    case SEARCHER_GREY_MODE:
        save_searcher_order(mode);
        break;
    }
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
    QObject::connect(s, SIGNAL(section_resized(int, int, int)), SIGNAL(resize_section(int, int, int)));
    QObject::connect(s, SIGNAL(section_moved(int)), SIGNAL(move_section(int)));
}



