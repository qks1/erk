#include "helpers.h"


//--------------------------------------------------------------------------//
//------------------ ФУНКЦИИ ВЫДАЧИ СООБЩЕНИЙ ОБ ОШИБКАХ -------------------//
//--------------------------------------------------------------------------//

// критическая ошибка, после которой требуется завершить программу
void critical_error(QString title, QString text){
    int ret = QMessageBox::critical(0, title, text);
    if(ret == QMessageBox::Ok)
        exit(EXIT_FAILURE);
}

// ошибка, после которой не требуется завершать программу
void error(QString title, QString text){
    QMessageBox::warning(0, title, text);
}

// Сообщение с вопросом (да/нет)
bool question(QString title, QString text){
    int ret = QMessageBox::warning(0, title, text, "Да", "Отмена", QString(), 0, 1);
    if(ret == 0){
        return true;
    }
    else{
        return false;
    }
}

//-------------------------------------------------------------------------------//


// функция соединения с базой данных.
bool create_connection(QSqlDatabase db){
    QSettings *settings = get_comp_settings();
    settings->beginGroup("DATABASE");
    QString DB_NAME = settings->value("basename", "erkbase").toString();
    QString DB_USER = settings->value("baseuser", "postgres").toString();
    QString DB_PASS = settings->value("basepass", "aurora2174_").toString();
    QString DB_HOST = settings->value("basehost", "192.168.1.101").toString();
    settings->endGroup();
    // перезаписываем настройки, если что-то было пусто
    settings->beginGroup("DATABASE");
    if(settings->value("basename").toString() != DB_NAME)
        settings->setValue("basename", DB_NAME);
    if(settings->value("basehost").toString() != DB_HOST)
        settings->setValue("basehost", DB_HOST);
    if(settings->value("baseuser").toString() != DB_USER)
        settings->setValue("baseuser", DB_USER);
    if(settings->value("basepass").toString() != DB_PASS)
        settings->setValue("basepass", DB_PASS);
    settings->endGroup();
    delete settings;

    // устанавливаем параметры, хранящиеся в файле constants
    db.setDatabaseName(DB_NAME);
    db.setUserName(DB_USER);
    db.setPassword(DB_PASS);
    db.setHostName(DB_HOST);

    // если db.open завершается неудачно, выдаём ошибку и выходим из программы
    if(!(db.open())){
        //QMessageBox::warning(0, "Ошибкаss", "Не удалось подключиться к базе данных. Проверьте настройки.\n" + db.lastError().text(), QMessageBox::Ok);
        return false;
    }

    // если с базой соединились, возвращаем true
    return true;
}

// функция установки основного шрифта приложения
void set_system_font(){
    QSettings *settings = get_comp_settings();
    settings->beginGroup("FONTS");
    QFont app_font;
    app_font.setFamily(settings->value("system_family", "Tahoma").toString());
    app_font.setPointSize(settings->value("system_size", 9).toInt());
    settings->endGroup();
    delete settings;
    qApp->setFont(app_font);
}

// функция замены символов в тексте при поиске
QString replases(QString text){
    text.replace("?", "_");
    text.replace("*", "%");
    text.replace(",", ".");
    return text.toUpper();
}

QString sorting_order_to_string(QList<SortingOrder> order){
    QStringList str;
    foreach(SortingOrder s, order)
        str << s.column + (s.order == Qt::AscendingOrder ? " ASC" : " DESC");
    return str.join(", ");
}

int find_index(QAbstractItemModel *model, QString str){
    for(int i=0; i < model->rowCount(); i++){
        if(model->data(model->index(i,0)).toString() == str)
            return i;
    }
    return -1;
}

//
bool operator ==(SortingOrder left, SortingOrder right){
    return (left.column == right.column);
}

bool add_groups(QTreeWidget *groups){
    // выбираем список групп из БД
    QSqlQuery query(base);
    QString strSelect = "SELECT id, name FROM " + GROUPS_TABLE + " ORDER BY name";

    // если не удаётся выполнить запрос, выдаём ошибку
    if(!query.exec(strSelect)){
        error("Ошибка", QString("Не удалось получить список групп:\n").append(strSelect));
        return false;
    }

    QString groupName;                              // название группы - для отображения в каталоге
    int groupNum;                                   // номер группы - для использования в фильтре
    QTreeWidgetItem* groupItem = 0;                 // элемент дерева

    // начинаем разбирать результат запроса
    QSqlRecord rec = query.record();
    while(query.next()){
        groupName = query.value(rec.indexOf("name")).toString();
        groupNum = query.value(rec.indexOf("id")).toInt();

        // формируем элемент дерева
        groupItem = new QTreeWidgetItem(groups);
        groupItem->setText(0, groupName);                           // устанавливаем groupName в качестве текста
        QStringList data;                                           // для передачи номера в фильтр прикрепляем данные к элементу
        data << QString::number(groupNum+ENLARGER) << groupName;    // чтобы отличить группу от подгруппы в фильтре, искусственно увеличим хранящийся номер
        groupItem->setData(0, Qt::UserRole, data);

        if(!add_subgroup(groupItem, groupNum))                      // для каждой группы добавляем список подгрупп
            return false;                                           // если одна из попыток завершилась неудачей, прерываем процесс
    }

    return true;
}

//--------------------------------------------------------------------------//

bool add_subgroup(QTreeWidgetItem *groupItem, int groupNum){
    // выбираем из БД список подгрупп для группы groupNum
    QSqlQuery query(base);
    QString strSelect = "SELECT id, name FROM " + SUBGROUPS_TABLE + " WHERE group_id=" + QString::number(groupNum);

    // если не удаётся выполнить запрос, выдаём ошибку
    if(!query.exec(strSelect)){
        error("Ошибка", QString("Не удалось получить список подгрупп:\n").append(strSelect));
        return false;
    }

    // формируем элемент поддерева
    QString subgroupName;               // отображаемый текст
    int subgroupNum;                    // номер подгруппы для передачи в фильтр
    QTreeWidgetItem *subgroupItem = 0;  // элемент поддерева

    // начинаем разбирать результат запроса
    QSqlRecord rec = query.record();
    while(query.next()){
        subgroupName = query.value(rec.indexOf("name")).toString();
        subgroupNum = query.value(rec.indexOf("id")).toInt();

        // для элемента поддерева устанавливаем в качестве предка соответствующий элемент дерева
        subgroupItem = new QTreeWidgetItem(groupItem);

        // устанавливаем текст и данные для элемента поддерева
        subgroupItem->setText(0, subgroupName);
        QStringList data;
        data << QString::number(subgroupNum) << subgroupName;
        subgroupItem->setData(0, Qt::UserRole, data);
    }
    return true;
}

QSettings *get_settings(){
    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    return settings;
}

QSettings *get_comp_settings(){
    //QSettings *comp_settings = new QSettings("erk", "base");
    QSettings *comp_settings = new QSettings("/home/df/erk_global.ini", QSettings::IniFormat);
    return comp_settings;
}

bool get_privilege(Privileges::Type type){
    QSqlQuery query(base);
    if(!query.exec(QString("SELECT permissions FROM privileges WHERE usergroup_id = %1 AND case_number = %2").arg(USERGROUP).arg(type))){
        qDebug() << QString("Не удалось выполнить запрос (привилегии): %1").arg(query.lastError().text());
        return false;
    }
    if(query.size() != 1)
        return false;
    query.next();
    return query.value(0).toBool();
}

void init_vars(){
    GLOBAL_MODE = SEARCHER_WHITE_MODE;

    WHITE_TABLE_COLUMNS << COLUMN_WHITE_ID
                        << COLUMN_WHITE_PHOTO
                        << COLUMN_WHITE_YEARSDISCOUNTS
                        << COLUMN_WHITE_NAME
                        << COLUMN_WHITE_SUBGROUP
                        << COLUMN_WHITE_QUANTITY
                        //<< COLUMN_WHITE_RETAILPRICE
                        //<< COLUMN_WHITE_WHOLEPRICE
                        //<< COLUMN_WHITE_WHOLEBEGIN
                        //<< COLUMN_WHITE_WHOLEUNIT
                        << COLUMN_WHITE_PAR1
                        << COLUMN_WHITE_PAR2
                        << COLUMN_WHITE_PAR3
                        << COLUMN_WHITE_PAR4
                        << COLUMN_WHITE_PAR5
                        << COLUMN_WHITE_PAR6
                        << COLUMN_WHITE_PAR7
                        << COLUMN_WHITE_PAR8
                        << COLUMN_WHITE_PAR9
                        << COLUMN_WHITE_PAR10
                        << COLUMN_WHITE_PAR11
                        << COLUMN_WHITE_PAR12
                        << COLUMN_WHITE_UNIT
                        << COLUMN_WHITE_WEIGHT
                        << COLUMN_WHITE_WEIGHTUNIT
                        << COLUMN_WHITE_NOTES
                        << COLUMN_WHITE_CREATED
                        << COLUMN_WHITE_EDITED;
    if(get_privilege(Privileges::Prices_view_access)){
        // сами цены вставляем в серёдку, даты их изменения - в конец
        WHITE_TABLE_COLUMNS.insert(WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_QUANTITY)+1, COLUMN_WHITE_RETAILPRICE);
        WHITE_TABLE_COLUMNS.insert(WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_RETAILPRICE)+1, COLUMN_WHITE_WHOLEPRICE);
        WHITE_TABLE_COLUMNS.insert(WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_WHOLEPRICE)+1, COLUMN_WHITE_WHOLEBEGIN);
        WHITE_TABLE_COLUMNS << COLUMN_WHITE_RETAILUPDATE
                            << COLUMN_WHITE_WHOLEUPDATE
                            << COLUMN_WHITE_WHOLEBEGINUPDATE;
    }
    GREY_TABLE_COLUMNS << COLUMN_GREY_ID
                       << COLUMN_GREY_NAME
                       << COLUMN_GREY_QUANTITY
                       << COLUMN_GREY_YEAR
                       << COLUMN_GREY_RETAILPRICE
                       << COLUMN_GREY_STORAGE
                       << COLUMN_GREY_RACK
                       << COLUMN_GREY_BOARD
                       << COLUMN_GREY_BOX
                       << COLUMN_GREY_INSPECTION
                       << COLUMN_GREY_ADDINFO
                       << COLUMN_GREY_DEFECT
                       << COLUMN_GREY_CATEGORY;

    WHITE_COLUMNS_NAMES["id"] = "id";
    WHITE_COLUMNS_NAMES["name"] = "Имя";
    WHITE_COLUMNS_NAMES["subgroup"] = "Группа";
    WHITE_COLUMNS_NAMES["quantity"] = "Кол-во";
    WHITE_COLUMNS_NAMES["price_ret"] = "Розн. цена";
    WHITE_COLUMNS_NAMES["price_whole"] = "Опт. цена";
    WHITE_COLUMNS_NAMES["whole_begin"] = "Опт с";
    WHITE_COLUMNS_NAMES["par1_val"] = "1";
    WHITE_COLUMNS_NAMES["par2_val"] = "2";
    WHITE_COLUMNS_NAMES["par3_val"] = "3";
    WHITE_COLUMNS_NAMES["par4_val"] = "4";
    WHITE_COLUMNS_NAMES["par5_val"] = "5";
    WHITE_COLUMNS_NAMES["par6_val"] = "6";
    WHITE_COLUMNS_NAMES["par7_val"] = "7";
    WHITE_COLUMNS_NAMES["par8_val"] = "8";
    WHITE_COLUMNS_NAMES["par9_val"] = "9";
    WHITE_COLUMNS_NAMES["par10_val"] = "10";
    WHITE_COLUMNS_NAMES["par11_val"] = "11";
    WHITE_COLUMNS_NAMES["par12_val"] = "12";
    WHITE_COLUMNS_NAMES["photo"] = "Фото";
    WHITE_COLUMNS_NAMES["unit_name"] = "Ед.изм.";
    WHITE_COLUMNS_NAMES["weight"] = "Вес";
    WHITE_COLUMNS_NAMES["notes"] = "Примечание";
    WHITE_COLUMNS_NAMES["created"] = "Создан";
    WHITE_COLUMNS_NAMES["edited"] = "Изменён";
    WHITE_COLUMNS_NAMES["retail_update"] = "Розн. цена изм.";
    WHITE_COLUMNS_NAMES["whole_update"] = "Опт. цена изм.";
    WHITE_COLUMNS_NAMES["begin_update"] = "Опт с изм.";
    WHITE_COLUMNS_NAMES["years_discounts"] = "Г";

    GREY_COLUMNS_NAMES["id"] = "id";
    GREY_COLUMNS_NAMES["quantity"] = "Кол-во";
    GREY_COLUMNS_NAMES["year"] = "Год";
    GREY_COLUMNS_NAMES["name"] = "Имя";
    GREY_COLUMNS_NAMES["storage"] = "Склад";
    GREY_COLUMNS_NAMES["rack"] = "Стеллаж";
    GREY_COLUMNS_NAMES["board"] = "Полка";
    GREY_COLUMNS_NAMES["box"] = "Ящик";
    GREY_COLUMNS_NAMES["insp_name"] = "Приёмка";
    GREY_COLUMNS_NAMES["add_info"] = "Доп.пар.1";
    GREY_COLUMNS_NAMES["defect"] = "Доп.пар.2";
    GREY_COLUMNS_NAMES["category_name"] = "Категория";
    GREY_COLUMNS_NAMES["price_ret"] = "Цена";

    // текст для пункта "любой" в комбобоксе
    ANY_ITEM_TEXT = "<любой>";
    // чем заполняются пустые фильтры по местам
    NOPLACE_TEXT = "NOPLACE";
    // пустые фильтры по доп. параметрам
    NOPAR_TEXT = "NOPAR";
    // отсутствие года
    NOYEAR_TEXT = "бг";
    // пустые фильтры по доп. параметрам
    NOINFO_TEXT = "NOINFO";

    QSettings *comp_settings = get_comp_settings();
    SETTINGS_PATH = comp_settings->value("path", "D:\erk.ini").toString();
    if(comp_settings->value("path").toString() != SETTINGS_PATH)
        comp_settings->setValue("path", SETTINGS_PATH);

    QSettings *settings = get_settings();

    USERNAME = "admin";
    PHOTOS_PATH = settings->value(QString("%1/PHOTOS_PATH").arg(USERNAME), "\\\\192.168.1.101\\config\\BMP\\").toString();
    if(settings->value(QString("%1/PHOTOS_PATH").arg(USERNAME)).toString() != PHOTOS_PATH)
        settings->setValue(QString("%1/PHOTOS_PATH").arg(USERNAME), PHOTOS_PATH);
    if(PHOTOS_PATH.at(PHOTOS_PATH.size()-1) != QDir::separator())
        PHOTOS_PATH += QDir::separator();
    settings->sync();
    delete settings;
    delete comp_settings;


    DEFAULT_WEIGHT_UNIT = "кг";
    DEFAULT_UNIT = "шт.";

    is_connect = false;
}
