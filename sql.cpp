#include <QtGui>
#include <QtSql>
#include "searcher.h"
#include "helpers.h"

int main(int argc, char **argv){
    QApplication app(argc,argv);

    // устанавливаем кодировку строк
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));

    // создаём объект базы данных и соединяемся с ним
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
	if(!createConnection(db))
        // если createConnection вернула false, чего быть не может, но мало ли, то завершаем программу
        return -1;

    Searcher *mainWidget = new Searcher();
    mainWidget->setWindowState(Qt::WindowMaximized);
    mainWidget->show();

    // запускаем приложение
	return app.exec();
}
