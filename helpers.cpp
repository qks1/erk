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
bool createConnection(QSqlDatabase db){
    /*QString DB_NAME = "erkbase";
    QString DB_USER = "df";
    QString DB_PASSWORD = "j39ljzuzy";
    QString DB_HOST = "localhost";*/
    // устанавливаем параметры, хранящиеся в файле constants
    db.setDatabaseName(DB_NAME);
    db.setUserName(DB_USER);
    db.setPassword(DB_PASSWORD);
    db.setHostName(DB_HOST);

    // если db.open завершается неудачно, выдаём ошибку и выходим из программы
    if(!(db.open()))
        error("Ошибка", QString("Не удалось подключиться к базе данных:\n").append(db.lastError().text()));

    // если с базой соединились, возвращаем true
    return true;
}

// функция установки основного шрифта приложения
void set_system_font(QString family, int size){
    QFont app_font;
    app_font.setFamily(family);
    app_font.setPixelSize(size);
    qApp->setFont(app_font);
}

// функция замены символов в тексте при поиске
QString replases(QString text){
    text.replace("?", "_");
    text.replace("*", "%");
    text.replace(",", ".");
    return text.toUpper();
}

//
bool operator ==(SortingOrder left, SortingOrder right){
    return (left.column == right.column);
}
