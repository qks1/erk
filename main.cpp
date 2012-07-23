#include <QtGui>
#include <QtSql>
#include "searcher.h"
#include "helpers.h"
#include "constants.h"
#include "mainwindow.h"

int main(int argc, char **argv){
    QApplication app(argc,argv);

    // устанавливаем кодировку строк
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));

    // создаём объект базы данных и соединяемся с ним
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
	if(!createConnection(db))
        // если createConnection вернула false, чего быть не может, но мало ли, то завершаем программу
        return -1;

    // создаём стиль для табвиджета, устанавливающий размер вкладок
    app.setStyleSheet("QTabBar::tab { min-width:50px; max-height:20px}");

    set_system_font(SYSTEM_FONT_FAMILY, SYSTEM_FONT_SIZE);


    // создаём главное окно
    MainWindow *mw = new MainWindow();
    mw->setWindowState(Qt::WindowMaximized);
    mw->show();
    //mw->setMaximumSize(mw->width(), mw->height());

    QObject::connect(mw, SIGNAL(exit_signal()),
            &app, SLOT(closeAllWindows()));

    // запускаем приложение
	return app.exec();
}
