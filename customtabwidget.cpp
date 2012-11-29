#include "customtabwidget.h"

CustomTabWidget::CustomTabWidget(QWidget *parent) : QTabWidget(parent){
    max = 0;

    QObject::connect(this, SIGNAL(tabCloseRequested(int)), SLOT(close_tab(int)));
    QObject::connect(this, SIGNAL(currentChanged(int)), SLOT(set_reserve_header()));

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

Searcher* CustomTabWidget::add_tab(ReserveStruct rstruct,
                                   ColumnsStruct *white_columns,
                                   ColumnsStruct *grey_columns,
                                   ColumnsStruct *reserve_columns){
    Searcher *s = new Searcher(rstruct, white_columns, grey_columns, reserve_columns, this);
    this->max++;
    this->addTab(s, QString::number(this->max));
    QObject::connect(s, SIGNAL(section_resized(int, int, int, QString)), SLOT(save_width(int,int,int, QString)));
    QObject::connect(s, SIGNAL(section_moved(int, int, int, QString)), SLOT(save_order(int,int,int, QString)));
    QObject::connect(s, SIGNAL(open_settings()), SIGNAL(open_settings()));
    QObject::connect(s, SIGNAL(need_white_refresh()), SLOT(refresh_white_searcher()));
    QObject::connect(s, SIGNAL(need_grey_refresh()), SLOT(refresh_grey_searcher()));
    QObject::connect(s, SIGNAL(need_blue_refresh()), SLOT(refresh_blue_searcher()));
    QObject::connect(s, SIGNAL(need_total_refresh()), SLOT(refresh_searcher()));
    QObject::connect(s, SIGNAL(rename_tab(QString, bool)), SLOT(set_tab_name(QString, bool)));
    QObject::connect(s, SIGNAL(rename_tab(QString)), SLOT(set_tab_name(QString)));
    QObject::connect(s, SIGNAL(switch_reserve_signal()), SLOT(switch_reserves()));
    return s;
}

void CustomTabWidget::save_searcher_width(int mode, int index, int width){
    Searcher *s;
    for(int i = 0; i < search_tabs->count(); i++){
        s = static_cast<Searcher*>(search_tabs->widget(i));
        if(mode == SEARCHER_WHITE_MODE) s->restore_white_width(index, width);
        else if(mode == SEARCHER_GREY_MODE) s->restore_grey_width(index, width);
    }
}

void CustomTabWidget::save_searcher_order(int mode, int logical, int newvisual){
    Searcher *s;
    for(int i = 0; i < search_tabs->count(); i++){
        s = static_cast<Searcher*>(search_tabs->widget(i));
        if(mode == SEARCHER_WHITE_MODE) s->restore_white_order(logical, newvisual);
        else if(mode == SEARCHER_GREY_MODE) s->restore_grey_order(logical, newvisual);
    }
}

void CustomTabWidget::save_width(int mode, int index, int width, QString section){
    emit save_width_signal(index, width, section);
    switch(mode){
    case SEARCHER_WHITE_MODE:
        save_searcher_width(mode, index, width);
        break;
    case SEARCHER_GREY_MODE:
        save_searcher_width(mode, index, width);
        break;
    }
}

void CustomTabWidget::save_order(int mode, int logical, int newvisual, QString section){
    emit save_order_signal(logical, newvisual, section);
    switch(mode){
    case SEARCHER_WHITE_MODE:
        save_searcher_order(mode, logical, newvisual);
        break;
    case SEARCHER_GREY_MODE:
        save_searcher_order(mode, logical, newvisual);
        break;
    }
}

void CustomTabWidget::switch_reserves(){
    searcher_show_reserve = !searcher_show_reserve;
    //qDebug() << searcher_show_reserve;
    Searcher *s;
    for(int i = 0; i < search_tabs->count(); i++){
        s = static_cast<Searcher*>(search_tabs->widget(i));
        s->switch_reserves();
    }
}

void CustomTabWidget::refresh_white_searcher(){
    Searcher *s;
    for(int i = 0; i < search_tabs->count(); i++){
        s = static_cast<Searcher*>(search_tabs->widget(i));
        s->refresh_white_table();
    }
}

void CustomTabWidget::refresh_grey_searcher(){
    Searcher *s;
    for(int i = 0; i < search_tabs->count(); i++){
        s = static_cast<Searcher*>(search_tabs->widget(i));
        s->refresh_grey_table();
    }
}

void CustomTabWidget::refresh_blue_searcher(){
    Searcher *s;
    for(int i = 0; i < search_tabs->count(); i++){
        s = static_cast<Searcher*>(search_tabs->widget(i));
        s->refresh_blue_table();
    }
}

void CustomTabWidget::refresh_searcher(){
    refresh_white_searcher();
    refresh_grey_searcher();
    refresh_blue_searcher();
}

void CustomTabWidget::set_reserve_header(){
    Searcher *s = (Searcher *)this->currentWidget();
    s->set_reserve_header();
}

void CustomTabWidget::set_tab_name(QString name, bool save){
    if(save)
        old_tab_name = this->tabText(this->currentIndex());
    this->setTabText(this->currentIndex(), name.length() == 0 ? QString::number(this->currentIndex()+1) : name);
}
