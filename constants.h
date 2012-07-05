#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QtGui>

// Описываются все глобальные переменные, используемые в проекте.
// Их назначение и значения - в constants.cpp

extern QString DB_NAME;
extern QString DB_USER;
extern QString DB_PASSWORD;
extern QString DB_HOST;

extern QString GROUPS_TABLE;
extern QString SUBGROUPS_TABLE;
extern QString TOVMARKS_TABLE;

extern QFont SMALL_FONT;
extern int SMALL_FONT_SIZE;

extern int ITEMS_ON_PAGE;

extern int ENLARGER;

extern QPixmap first_icon;
extern QPixmap prev_icon;
extern QPixmap next_icon;
extern QPixmap last_icon;

extern int PAGE_FIRST;
extern int PAGE_PREV;
extern int PAGE_NEXT;
extern int PAGE_LAST;


void init_vars();


#endif // CONSTANTS_H
