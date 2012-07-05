#include "constants.h"

// параметры соединения с БД
QString DB_NAME = "erkbase";
QString DB_USER = "df";
QString DB_PASSWORD = "j39ljzuzy";
QString DB_HOST = "localhost";

// таблицы БД
QString GROUPS_TABLE = "groups";
QString SUBGROUPS_TABLE = "subgroups";
QString TOVMARKS_TABLE = "tovmarks";

// настройки поисковика
int ITEMS_ON_PAGE = 50;



// прочее

// в поисковике выборка может производиться как по группам, так и по подгруппам.
// чтобы в фильтре отличить группу от подгруппы, в каталоге (дереве) к номеру каждой группы прибавляется большое число.
int ENLARGER = 100000;
