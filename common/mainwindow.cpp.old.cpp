#include "mainwindow.h"
#include <QMetaType>

QDataStream &operator<<(QDataStream &out, const ColumnsStruct &obj)
{
     out << obj.sizes << obj.indices << obj.appears;
     return out;
}

QDataStream &operator>>(QDataStream &in, ColumnsStruct &obj)
{
    in >> obj.sizes >> obj.indices >> obj.appears;
    return in;
}


MainWindow::MainWindow()
{
    // Чтобы можно было записывать и считывать из настроек структуры ColumnsStruct,
    // зарегистрируем метатипы и stream operators (определения самих операторов внизу файла)
    qRegisterMetaType<ColumnsStruct>("ColumnsStruct");
    qRegisterMetaTypeStreamOperators<ColumnsStruct>("ColumnsStruct");

    init_vars();
    this->setWindowTitle("ООО \"ЭРК\"");
    search_tabs = 0;
    mwidget = new QStackedWidget(this);
    settings = get_settings();
    comp_settings = get_comp_settings();

    reserve_struct.source_model = new QSqlQueryModel(this);
    reserve_struct.list_model = new QStandardItemModel(10, 1, this);
    reserve_struct.selection_model = new QItemSelectionModel(reserve_struct.list_model);
    reserve_struct.table_header = new QHeaderView(Qt::Horizontal);

    // считываем из settings настройки столбцов (ширину, индексы, показываются ли).

    white_columns = form_columns_struct("WHITE_COLUMNS");
    grey_columns = form_columns_struct("GREY_COLUMNS");
    reserve_columns = form_columns_struct("RESERVE_COLUMNS");

    // заполняем reserve.list_model заявками по числу max_reserves
    for(int i = 0; i < max_reserves; i++){
        reserve_struct.list_model->setData(reserve_struct.list_model->index(i, 0), QString("Заявка %1").arg(i+1));
        reserve_struct.list_model->setData(reserve_struct.list_model->index(i, 0), QString::number(i+1), Qt::UserRole);
        if(count_reserve_doc_sum(i+1) > 0)
            reserve_struct.list_model->setData(reserve_struct.list_model->index(i, 0), Qt::red, Qt::TextColorRole);
    }
    this->setCentralWidget(mwidget);
    // создаём поисковик
    create_search_widget();

    // создаём действия
    create_actions();

    // создаём меню
    create_menu();

}

MainWindow::~MainWindow(){
}

void MainWindow::closeEvent(QCloseEvent *){
    // тут надо записать все столбцы
    save_columns_struct(white_columns, "WHITE_COLUMNS");
    save_columns_struct(grey_columns, "GREY_COLUMNS");
    save_columns_struct(reserve_columns, "RESERVE_COLUMNS");
    settings->sync();

    delete mwidget;
    delete settings;
    delete comp_settings;
    base.close();

    qDebug() << "Screw you guys! I'm going home.";
}

inline ColumnsStruct MainWindow::form_columns_struct(QString section){
    /* Функция формирует структуру столбцов (списки размеров, индексов и показываемых столбцов), считывая их из настроек.
       Аргумент - секция настроек, из которой будем считывать.
       Сами настройки - элемент класса section.
       Структура состоит из трёх списков (sizes, indices, appears) и записана в настройках сразу целиком.
    */
    ColumnsStruct structure;
    settings->beginGroup(QString("%1").arg(USERNAME));
    structure = settings->value(section, QVariant()).value<ColumnsStruct>();
    settings->endGroup();


    return structure;
}

inline void MainWindow::save_columns_struct(ColumnsStruct structure, QString section){
    ColumnsStruct strrtr;
    settings->beginGroup(QString("%1").arg(USERNAME));
    settings->setValue(section, QVariant::fromValue<ColumnsStruct>(structure));
    strrtr = settings->value(section, QVariant()).value<ColumnsStruct>();
    settings->endGroup();
}

void MainWindow::save_width(int index, int width, QString section){
    qDebug() << index << width;
    if(section == "WHITE_COLUMNS")
        white_columns.sizes[index] = width;
    else if(section == "GREY_COLUMNS")
        grey_columns.sizes[index] = width;
    else if(section == "RESERVE_COLUMNS")
        reserve_columns.sizes[index] = width;
}

void MainWindow::save_order(int logical, int newvisual, QString section){
    // так как при перемещении столбцов меняются обычно несколько индексов, уместно переписать их все
}

double MainWindow::count_reserve_doc_sum(int num){
    QSqlQuery query(base);
    QString query_str = QString("SELECT sum(price*quantity) "
                                "FROM reserve "
                                "WHERE doc_id = (SELECT id FROM reserve_docs WHERE user_id = %1 AND doc_num = %2)")
                                .arg(USER_ID).arg(num);
    if(!query.exec(query_str)){
        qDebug() << query.lastError().text();
        return 0;
    }
    if(query.size() == 0)
        return 0;
    query.next();
    return query.value(0).toDouble();
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

void MainWindow::refresh_white_tables(){
    Searcher *s;
    for(int i = 0; i < search_tabs->count(); i++){
        s = static_cast<Searcher*>(search_tabs->widget(i));
        s->refresh_white_table();
    }
}

void MainWindow::refresh_grey_tables(){
    Searcher *s;
    for(int i = 0; i < search_tabs->count(); i++){
        s = static_cast<Searcher*>(search_tabs->widget(i));
        s->refresh_grey_table();
    }
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
    QObject::connect(addtab, SIGNAL(clicked()), this, SLOT(add_tab()));
    search_tabs->setCornerWidget(addtab, Qt::TopRightCorner);
    // на каждый таб создадим свой Searcher
    // кол-во вкладок по умолчанию устанавливается в constants.cpp
    Searcher *s;
    for(int i = 0; i < settings->value(QString("%1/tabs_default").arg(USERNAME), 5).toInt(); i++){
        s = search_tabs->add_tab(reserve_struct, &white_columns, &grey_columns, &reserve_columns);
        if(s->success == false)
            break;
        QObject::connect(s, SIGNAL(open_settings()), this, SLOT(send_settings()));
        QObject::connect(s, SIGNAL(catalog_hides()), this, SLOT(uncheck_menu_catalog()));
        QObject::connect(s, SIGNAL(catalog_shows()), this, SLOT(check_menu_catalog()));
        QObject::connect(s, SIGNAL(white_columns_changed()), this, SLOT(send_columns()));
        QObject::connect(s, SIGNAL(grey_columns_changed()), this, SLOT(send_columns()));
        //QObject::connect(s, SIGNAL(need_white_refresh()), this, SLOT(refresh_white_tables()));
        //QObject::connect(s, SIGNAL(need_grey_refresh()), this, SLOT(refresh_grey_tables()));
    }
    s = (Searcher *)search_tabs->widget(0);
    s->set_reserve_header();
    mwidget->addWidget(search_tabs);

    connect(search_tabs, SIGNAL(save_width_signal(int,int,QString)), SLOT(save_width(int,int,QString)));
    connect(search_tabs, SIGNAL(save_order_signal(int,int,QString)), SLOT(save_order(int,int,QString)));
}

void MainWindow::add_tab(){
    search_tabs->add_tab(reserve_struct, &white_columns, &grey_columns, &reserve_columns);
}

void MainWindow::check_menu_catalog(){
    showcatalog_action->setChecked(true);
}

void MainWindow::uncheck_menu_catalog(){
    showcatalog_action->setChecked(false);
}



