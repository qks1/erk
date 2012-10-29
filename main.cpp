#include <QtGui>
#include <QtSql>
#include "helpers.h"
#include "constants.h"
#include "mainwindow.h"
#include "authorisewindow.h"

void create_mainwindow(){

}

int main(int argc, char **argv){
    QApplication app(argc,argv);

    // устанавливаем кодировку строк
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    //init_vars();

    // создаём объект базы данных и соединяемся с ним
    base = QSqlDatabase::addDatabase("QPSQL", "base");
    bool success = create_connection(base);

    // создаём стиль для табвиджета, устанавливающий размер вкладок
    app.setStyleSheet("QTabBar::tab { min-width:50px; max-height:20px}");

    set_system_font();

    AuthoriseWindow auth(success);
    QObject::connect(&auth, SIGNAL(close_signal()), &app, SLOT(closeAllWindows()));
    auth.exec();

    // запускаем приложение
	return app.exec();
}
