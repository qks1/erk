#include "constants.h"
#include <QtGui>

// параметры соединения с БД
QString DB_NAME = "postgres";
QString DB_USER = "postgres";
QString DB_PASSWORD = "aurora2174_";
QString DB_HOST = "192.168.1.101";

// таблицы БД
QString GROUPS_TABLE = "groups";
QString SUBGROUPS_TABLE = "subgroups";
QString TOVMARKS_TABLE = "tovmarks";

// мелкий шрифт (для переключателя страниц)
QFont SMALL_FONT;
int SMALL_FONT_SIZE = 10;

// настройки поисковика
int ITEMS_ON_PAGE = 50;

// константы для передачи сигнала о смене номера страницы в переключателе
int PAGE_FIRST = 1;
int PAGE_PREV = 2;
int PAGE_NEXT = 3;
int PAGE_LAST = 4;



// прочее

// в поисковике выборка может производиться как по группам, так и по подгруппам.
// чтобы в фильтре отличить группу от подгруппы, в каталоге (дереве) к номеру каждой группы прибавляется большое число.
int ENLARGER = 100000;


void init_vars(){
    SMALL_FONT.setPixelSize(SMALL_FONT_SIZE);
}
