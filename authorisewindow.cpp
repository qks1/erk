#include "authorisewindow.h"
#include "ui_authorisewindow.h"

AuthoriseWindow::AuthoriseWindow(bool success, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthoriseWindow)
{
    ui->setupUi(this);
    this->adjustSize();
    this->setFixedSize(this->size());
    //this->ui->pass_text->setFocus();

    // ширина кнопки "настройки..."
    this->ui->settings_button->setMaximumWidth(this->ui->settings_button->fontMetrics().width('_') * (this->ui->settings_button->text().length() + 2));

    // заголовок окна
    this->setWindowTitle("Авторизация");

    // если с базой не соединились, просим проверить настройки, иначе заполняем бокс с пользователями
    if(!success){
        QMessageBox::warning(this, "Ошибка", QString("Не удалось соединиться с базой данных. Проверьте настройки.\n%1").arg(base.lastError().text()), QMessageBox::Ok);
        this->ui->enter_button->setEnabled(false);
    }
    else{
        fill_users_box();
    }

    QObject::connect(this->ui->close_button, SIGNAL(clicked()), SLOT(close_slot()));
    QObject::connect(this->ui->enter_button, SIGNAL(clicked()), SLOT(enter_slot()));
    QObject::connect(this->ui->settings_button, SIGNAL(clicked()), SLOT(settings_slot()));
    QObject::connect(this->ui->name_box, SIGNAL(activated(int)), SLOT(set_focus_slot(int)));
}

AuthoriseWindow::~AuthoriseWindow()
{
    delete ui;
}

void AuthoriseWindow::closeEvent(QCloseEvent *e){
    QDialog::closeEvent(e);
    exit(0);
}

void AuthoriseWindow::keyPressEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_Return)
        enter_slot();
    else if(e->key() == Qt::Key_Escape)
        close_slot();
    else
        QDialog::keyPressEvent(e);
}

void AuthoriseWindow::set_focus_slot(int n){
    this->ui->pass_text->setFocus();
}

void AuthoriseWindow::fill_users_box(){
    QSqlQuery query(base);
    if(!query.exec("SELECT id, login FROM users ORDER BY login")){
        error("Ошибка", QString("Не удалось выполнить запрос:\n%1").arg(query.lastError().text()));
        return;
    }
    while(query.next()){
        this->ui->name_box->addItem(query.value(1).toString(), query.value(0));
    }
}

void AuthoriseWindow::close_slot(){
    emit close_signal();
    QDialog::reject();
}

void AuthoriseWindow::enter_slot(){
    int index = this->ui->name_box->currentIndex();
    if(index < 0 || this->ui->name_box->currentText() != this->ui->name_box->itemText(index))
        error("Ошибка", "Пользователя с таким именем не существует");
    else{
        int id = this->ui->name_box->itemData(index).toInt();
        QSqlQuery query(base);
        if(!query.exec(QString("SELECT login, password, usergroup_id FROM users WHERE id = %1").arg(id))){
            error("Ошибка", QString("Не удалось выполнить запрос:\n%1").arg(query.lastError().text()));
            return;
        }
        query.next();
        if(query.value(1).toString() != QCryptographicHash::hash(this->ui->pass_text->text().toUtf8(), QCryptographicHash::Md5).toHex()){
            error("Ошибка", "Неверный пароль");
            return;
        }
        USERNAME = query.value(0).toString();
        USER_ID = id;
        // USERGROUP
        switch(query.value(2).toInt()){
        case 1:
            USERGROUP = Usergroups::Administrator;
            break;
        case 2:
            USERGROUP = Usergroups::Senior_manager;
            break;
        case 3:
            USERGROUP = Usergroups::Manager;
            break;
        case 4:
            USERGROUP = Usergroups::Bookkeeper;
            break;
        case 5:
            USERGROUP = Usergroups::Stockman;
            break;
        default:
            QMessageBox::warning(this, "Ошибка идентификации", "не удалось установить группу пользователя", QMessageBox::Ok);
            return;
        }

        is_connect = true;
        QDialog::accept();
        create_mainwindow();
    }
}

void AuthoriseWindow::settings_slot(){
    AuthSettingsDialog *settings_dialog = new AuthSettingsDialog(this);
    if(settings_dialog->exec() == QDialog::Accepted){
        this->ui->enter_button->setEnabled(true);
        this->ui->name_box->clear();
        fill_users_box();
    }
}

void AuthoriseWindow::create_mainwindow(){
    // создаём главное окно
    MainWindow *mw = new MainWindow();
    mw->setWindowState(Qt::WindowMaximized);
    mw->show();
    //mw->setMaximumSize(mw->width(), mw->height());

    QObject::connect(mw, SIGNAL(exit_signal()), qApp, SLOT(closeAllWindows()));
}

AuthSettingsDialog::AuthSettingsDialog(QWidget *parent, Qt::WindowFlags) :
    QDialog(parent)
{
    settings = get_comp_settings();
    need_restart = false;
    QVBoxLayout *layout = new QVBoxLayout();

    settings->beginGroup("DATABASE");
    old_basename = settings->value("basename").toString();
    old_basehost = settings->value("basehost").toString();
    old_baseuser = settings->value("baseuser").toString();
    old_basepass = settings->value("basepass").toString();

    base_name = new QLineEdit(old_basename);
    base_host = new QLineEdit(old_basehost);
    base_user = new QLineEdit(old_baseuser);
    base_pass = new QLineEdit(old_basepass);
    settings->endGroup();
    base_pass->setEchoMode(QLineEdit::Password);

    QGridLayout *db_layout  = new QGridLayout();
    db_layout->addWidget(new QLabel("Название БД"), 0, 0);
    db_layout->addWidget(new QLabel("Хост"), 1, 0);
    db_layout->addWidget(new QLabel("Пользователь"), 2, 0);
    db_layout->addWidget(new QLabel("Пароль"), 3, 0);
    db_layout->addWidget(base_name, 0, 1);
    db_layout->addWidget(base_host, 1, 1);
    db_layout->addWidget(base_user, 2, 1);
    db_layout->addWidget(base_pass, 3, 1);

    // виджет с кнопками
    QWidget *btns = new QWidget(this);
    QPushButton *dialog_ok_button = new QPushButton("OK");
    QPushButton *dialog_cancel_button = new QPushButton("Отмена");
    QHBoxLayout *btns_layout = new QHBoxLayout();
    btns_layout->addWidget(dialog_ok_button);
    btns_layout->addWidget(dialog_cancel_button);
    btns->setLayout(btns_layout);

    layout->addLayout(db_layout);
    layout->addWidget(btns);
    this->setLayout(layout);
    this->setWindowTitle("Настройки базы");
    this->adjustSize();
    this->setFixedSize(this->size());

    QObject::connect(dialog_ok_button, SIGNAL(clicked()), SLOT(accept()));
    QObject::connect(dialog_cancel_button, SIGNAL(clicked()), SLOT(reject()));
}

AuthSettingsDialog::~AuthSettingsDialog(){
    delete settings;
}

void AuthSettingsDialog::accept(){
    settings->beginGroup("DATABASE");
    if(settings->value("basename").toString() != base_name->text()){
        settings->setValue("basename", base_name->text());
        need_restart = true;
    }
    if(settings->value("basehost").toString() != base_host->text()){
        settings->setValue("basehost", base_host->text());
        need_restart = true;
    }
    if(settings->value("baseuser").toString() != base_user->text()){
        settings->setValue("baseuser", base_user->text());
        need_restart = true;
    }
    if(settings->value("basepass").toString() != base_pass->text()){
        settings->setValue("basepass", base_pass->text());
        need_restart = true;
    }
    settings->endGroup();
    settings->sync();

    if(need_restart){
        base.close();
        if(!create_connection(base))
            error("Ошибка", QString("Не удалось соединиться с базой данных. Проверьте настройки.\n%1").arg(base.lastError().text()));
    }
    QDialog::accept();
}

void AuthSettingsDialog::reject(){
    if(need_restart){
        settings->beginGroup("DATABASE");
        settings->setValue("basename", old_basename);
        settings->setValue("basehost", old_basehost);
        settings->setValue("baseuser", old_baseuser);
        settings->setValue("basepass", old_basepass);
        settings->endGroup();
        if(!create_connection(base))
            error("Ошибка", QString("Не удалось соединиться с базой данных. Проверьте настройки.\n%1").arg(base.lastError().text()));
    }
    QDialog::reject();
}
