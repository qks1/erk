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
    QSettings settings("erk", "base");
    settings.beginGroup(QString(USERNAME) + "/DATABASE");
    QString DB_NAME = settings.value("basename", "erkbase").toString();
    QString DB_USER = settings.value("baseuser", "postgres").toString();
    QString DB_PASS = settings.value("basepass", "aurora2174_").toString();
    QString DB_HOST = settings.value("basehost", "192.168.1.101").toString();
    settings.endGroup();
    // перезаписываем настройки, если что-то было пусто
    settings.beginGroup(QString(USERNAME) + "/DATABASE");
    if(settings.value("basename").toString() != DB_NAME)
        settings.setValue("basename", DB_NAME);
    if(settings.value("basehost").toString() != DB_HOST)
        settings.setValue("basehost", DB_HOST);
    if(settings.value("baseuser").toString() != DB_USER)
        settings.setValue("baseuser", DB_USER);
    if(settings.value("basepass").toString() != DB_PASS)
        settings.setValue("basepass", DB_PASS);
    settings.endGroup();

    // устанавливаем параметры, хранящиеся в файле constants
    db.setDatabaseName(DB_NAME);
    db.setUserName(DB_USER);
    db.setPassword(DB_PASS);
    db.setHostName(DB_HOST);

    // если db.open завершается неудачно, выдаём ошибку и выходим из программы
    if(!(db.open())){
        QMessageBox::warning(0, "Ошибка", "Не удалось подключиться к базе данных. Проверьте настройки.\n" + db.lastError().text(), QMessageBox::Ok);
        return false;
    }

    // если с базой соединились, возвращаем true
    return true;
}

// функция установки основного шрифта приложения
void set_system_font(){
    QSettings settings("erk", "base");
    settings.beginGroup(QString(USERNAME) + "/FONTS");
    QFont app_font;
    app_font.setFamily(settings.value("system_family", "Tahoma").toString());
    app_font.setPointSize(settings.value("system_size", 9).toInt());
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

// чтение шрифтов

