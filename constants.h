#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QtGui>

// константы для переключателя страниц
#define PAGE_FIRST 1
#define PAGE_PREV 2
#define PAGE_NEXT 3
#define PAGE_LAST 4
#define PAGE_NUMBER 5

#define MULTIPAGE_MODE 10
#define SINGLEPAGE_MODE 11

// константы для последнего применённого фильтра
#define GROUPS_FILTER 100
#define COLUMNS_FILTER 101
#define LIMITS_FILTER 102
#define ID_FILTER 103
#define BEGIN_TEXT_FILTER 104
#define PARTS_TEXT_FILTER 105

// константы для режима поиска по id
#define EQUAL 200
#define GREATER_OR_EQUAL 201

// константы для режимов поиска (текстовое поле)
#define ID_MODE 300
#define BEGIN_MODE 301
#define PARTS_MODE 302
#define ERROR_MODE 303

// в поисковике выборка может производиться как по группам, так и по подгруппам.
// чтобы в фильтре отличить группу от подгруппы, в каталоге (дереве) к номеру каждой группы прибавляется большое число.
#define ENLARGER 100000

// глобальные переменные, используемые в проекте.
// Их назначение и значения - в constants.cpp
// В последствии все эти переменные будут храниться в QSettings

extern QString DB_NAME;
extern QString DB_USER;
extern QString DB_PASSWORD;
extern QString DB_HOST;

extern QString GROUPS_TABLE;
extern QString SUBGROUPS_TABLE;
extern QString TOVMARKS_TABLE;

extern int SMALL_FONT_SIZE;

extern int ITEMS_ON_PAGE;

extern int MAX_ITEMS_WITHOUT_WARNING;

extern int TABS_DEFAULT;


void init_vars();


#endif // CONSTANTS_H
