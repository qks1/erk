#include "customtabwidget.h"

CustomTabWidget::CustomTabWidget(QWidget *parent) : QTabWidget(parent){
    max = 0;

    QObject::connect(this, SIGNAL(tabCloseRequested(int)), SLOT(close_tab(int)));
    QObject::connect(this, SIGNAL(resize_section(int,int,int)), SLOT(save_width(int,int,int)));
    QObject::connect(this, SIGNAL(move_section(int)), SLOT(save_order(int)));

}

void CustomTabWidget::close_tab(int index){
    if(this->count() <= 1)
        return;
    if(QMessageBox::question(this, "Закрыть вкладку?", "Закрыть вкладку?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){
        QWidget *d = widget(index);
        this->removeTab(index);
        delete(d);
    }
}

Searcher* CustomTabWidget::add_tab(){
    Searcher *s = new Searcher(this);
    this->max++;
    this->addTab(s, QString::number(this->max));
    QObject::connect(s, SIGNAL(section_resized(int, int, int)), SIGNAL(resize_section(int, int, int)));
    QObject::connect(s, SIGNAL(section_moved(int)), SIGNAL(move_section(int)));
    QObject::connect(s, SIGNAL(open_settings()), SIGNAL(open_settings()));
    return s;
}

void CustomTabWidget::save_searcher_width(int mode, int index, int width){
    QString section;
    QSettings *settings = get_settings();
    if(mode == SEARCHER_WHITE_MODE) section = "WHITE_COLUMNS";
    else if(mode == SEARCHER_GREY_MODE) section = "GREY_COLUMNS";
    // сначала сохраняем настройки
    settings->beginGroup(QString("%1/%2").arg(USERNAME).arg(section));
    settings->setValue(QString("width[%1]").arg(index), width);
    settings->endGroup();
    settings->sync();
    delete settings;
    // теперь применяем их ко всем вкладкам
    Searcher *s;
    for(int i = 0; i < search_tabs->count(); i++){
        s = static_cast<Searcher*>(search_tabs->widget(i));
        if(mode == SEARCHER_WHITE_MODE) s->restore_white_width(index, width);
        else if(mode == SEARCHER_GREY_MODE) s->restore_grey_width(index, width);
    }
}

void CustomTabWidget::save_searcher_order(int mode){
    Searcher *s;
    for(int i = 0; i < search_tabs->count(); i++){
        s = static_cast<Searcher*>(search_tabs->widget(i));
        if(mode == SEARCHER_WHITE_MODE) s->restore_white_order();
        else if(mode == SEARCHER_GREY_MODE) s->restore_grey_order();
    }
}

void CustomTabWidget::save_width(int mode, int index, int width){
    switch(mode){
    case SEARCHER_WHITE_MODE:
        save_searcher_width(mode, index, width);
        break;
    case SEARCHER_GREY_MODE:
        save_searcher_width(mode, index, width);
        break;
    }
}

void CustomTabWidget::save_order(int mode){
    switch(mode){
    case SEARCHER_WHITE_MODE:
        save_searcher_order(mode);
        break;
    case SEARCHER_GREY_MODE:
        save_searcher_order(mode);
        break;
    }
}
