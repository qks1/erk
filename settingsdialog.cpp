#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, Qt::WindowFlags) :
    QDialog(parent)
{
    this->settings = new QSettings("erk", "base");
    // создаём виджет с настройками БД
    QWidget *wgt = new QWidget();

    // на виджет добавляем четыре параметра
    QGridLayout *db_layout = new QGridLayout();
    QLabel *dbname_label = new QLabel("Название БД");
    QLabel *dbhost_label = new QLabel("Хост");
    QLabel *dbuser_label = new QLabel("Пользователь");
    QLabel *dbpass_label = new QLabel("Пароль");
    QLabel *photopath_label = new QLabel("Папка с фото");
    settings->beginGroup(QString(USERNAME) + "/DATABASE");
    base_name = new QLineEdit(settings->value("basename").toString());
    base_host = new QLineEdit(settings->value("basehost").toString());
    base_user = new QLineEdit(settings->value("baseuser").toString());
    base_pass = new QLineEdit(settings->value("basepass").toString());
    base_pass->setEchoMode(QLineEdit::Password);
    settings->endGroup();
    photo_path = new QLineEdit(settings->value(QString("%1/PHOTOS_PATH").arg(USERNAME)).toString());
    path_button = new QPushButton("...");
    path_button->setFixedWidth(path_button->font().pointSize()*2);
    path_button->setFixedHeight(path_button->width());
    db_layout->addWidget(dbname_label, 0, 0);
    db_layout->addWidget(dbhost_label, 1, 0);
    db_layout->addWidget(dbuser_label, 2, 0);
    db_layout->addWidget(dbpass_label, 3, 0);
    db_layout->addWidget(photopath_label, 4, 0);
    db_layout->addWidget(base_name, 0, 1);
    db_layout->addWidget(base_host, 1, 1);
    db_layout->addWidget(base_user, 2, 1);
    db_layout->addWidget(base_pass, 3, 1);
    db_layout->addWidget(photo_path, 4, 1);
    db_layout->addWidget(path_button, 4, 2);
    base_name->setFixedWidth(250);
    base_host->setFixedWidth(250);
    base_user->setFixedWidth(250);
    base_pass->setFixedWidth(250);
    photo_path->setFixedWidth(250);
    // устанавливаем лайаут на виджет
    wgt->setLayout(db_layout);

    // создаём виджет с настройками шрифтов
    QWidget *font_wgt = new QWidget();
    QLabel *system_font_label = new QLabel("Шрифт в программе");
    QLabel *print_font_label = new QLabel("Шрифт для печати");
    system_font_list = new QFontComboBox();
    print_font_list = new QFontComboBox();
    system_font_sizes = new QSpinBox();
    print_font_sizes = new QSpinBox();
    QFont system_font, print_font;
    settings->beginGroup(QString(USERNAME) + "/FONTS");
    system_font.setFamily(settings->value("system_family", qApp->font().family()).toString());
    system_font.setPointSize(settings->value("system_size", qApp->font().pointSize()).toInt());
    print_font.setFamily(settings->value("print_family", qApp->font().family()).toString());
    print_font.setPointSize(settings->value("print_size", qApp->font().pointSize()).toInt());
    settings->endGroup();
    system_font_list->setCurrentFont(system_font);
    print_font_list->setCurrentFont(print_font);
    system_font_sizes->setRange(4,72);
    print_font_sizes->setRange(4,72);
    system_font_sizes->setValue(system_font.pointSize());
    print_font_sizes->setValue(print_font.pointSize());
    QGridLayout *font_lt = new QGridLayout();
    font_lt->addWidget(system_font_label, 0, 0);
    font_lt->addWidget(system_font_list, 0, 1);
    font_lt->addWidget(system_font_sizes, 0, 2);
    //font_lt->addWidget(print_font_label, 1, 0);
    //font_lt->addWidget(print_font_list, 1, 1);
    //font_lt->addWidget(print_font_sizes, 1, 2);
    system_font_list->setFixedWidth(200);
    //print_font_list->setFixedWidth(200);
    font_wgt->setLayout(font_lt);

    QObject::connect(this->path_button, SIGNAL(clicked()), SLOT(photo_slot()));

    // создаём табвиджет и добавляем туда две вкладки
    QTabWidget *tabs = new QTabWidget();
    tabs->addTab(font_wgt, "Шрифты");
    tabs->addTab(wgt, "База");
    tabs->adjustSize();

    // создаём кнопки OK и Отмена, создаём лайаут, куда добавляем табвиджет и эти кнопки
    QWidget *btns_wgt = new QWidget();
    QPushButton *ok_button = new QPushButton("OK");
    QPushButton *cancel_button = new QPushButton("Отмена");
    QHBoxLayout *btn_layout = new QHBoxLayout();
    btn_layout->addWidget(ok_button);
    btn_layout->addWidget(cancel_button);
    ok_button->setFixedWidth(100);
    cancel_button->setFixedWidth(100);
    btns_wgt->setLayout(btn_layout);
    btns_wgt->adjustSize();
    btns_wgt->setFixedSize(btns_wgt->size());
    QVBoxLayout *main_lt = new QVBoxLayout();
    main_lt->addWidget(tabs);
    main_lt->addWidget(btns_wgt);
    this->setLayout(main_lt);

    // устанавливаем заголовок окна
    this->setWindowTitle("Настройки");

    this->adjustSize();
    this->setFixedSize(this->width(), this->height());

    // соединяем кнопки с действиями
    QObject::connect(ok_button, SIGNAL(clicked()), SLOT(accept()));
    QObject::connect(cancel_button, SIGNAL(clicked()), SLOT(reject()));
}

void SettingsDialog::photo_slot(){
    QString path = QFileDialog::getExistingDirectory(this, "Выберите папку с фотографиями");
    photo_path->setText(path);
}

void SettingsDialog::accept(){
    // если нажата кнопка ОК, сохраняем настройки
    settings->beginGroup(QString(USERNAME) + "/DATABASE");
    if(settings->value("basename").toString() != base_name->text())
        settings->setValue("basename", base_name->text());
    if(settings->value("basehost").toString() != base_host->text())
        settings->setValue("basehost", base_host->text());
    if(settings->value("baseuser").toString() != base_user->text())
        settings->setValue("baseuser", base_user->text());
    if(settings->value("basepass").toString() != base_pass->text())
        settings->setValue("basepass", base_pass->text());
    settings->endGroup();

    if(settings->value(QString("%1/PHOTOS_PATH").arg(USERNAME)).toString() != photo_path->text())
        settings->setValue(QString("%1/PHOTOS_PATH").arg(USERNAME), photo_path->text());

    settings->beginGroup(QString(USERNAME) + "/FONTS");
    if(settings->value("system_family").toString() != system_font_list->currentText())
            settings->setValue("system_family", system_font_list->currentText());
    if(settings->value("system_size").toInt() != system_font_sizes->value())
            settings->setValue("system_size", system_font_sizes->value());
    if(settings->value("print_family").toString() != print_font_list->currentText())
            settings->setValue("print_family", print_font_list->currentText());
    if(settings->value("print_size").toInt() != print_font_sizes->value())
            settings->setValue("print_size", print_font_sizes->value());
    settings->endGroup();
    settings->sync();
    QDialog::accept();
}
