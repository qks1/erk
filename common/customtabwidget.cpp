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

Searcher* CustomTabWidget::add_tab(ReserveStruct *rstruct,
                                   ColumnsStruct *white_columns,
                                   ColumnsStruct *grey_columns){
    Searcher *s = new Searcher(rstruct, white_columns, grey_columns, this);
    this->max++;
    this->addTab(s, QString::number(this->max));
    QObject::connect(s, SIGNAL(section_resized(int, int, QString)), SLOT(save_width(int,int, QString)));
    QObject::connect(s, SIGNAL(section_moved(int, int, QString)), SLOT(save_order(int,int, QString)));
    QObject::connect(s, SIGNAL(open_settings()), SIGNAL(open_settings()));
    QObject::connect(s, SIGNAL(need_white_refresh()), SLOT(refresh_white_searcher()));
    QObject::connect(s, SIGNAL(need_grey_refresh()), SLOT(refresh_grey_searcher()));
    QObject::connect(s, SIGNAL(need_blue_refresh()), SLOT(refresh_blue_searcher()));
    QObject::connect(s, SIGNAL(need_total_refresh()), SLOT(refresh_searcher()));
    QObject::connect(s, SIGNAL(rename_tab(QString, bool)), SLOT(set_tab_name(QString, bool)));
    QObject::connect(s, SIGNAL(rename_tab(QString)), SLOT(set_tab_name(QString)));
    QObject::connect(s, SIGNAL(switch_reserve_signal()), SLOT(switch_reserves()));
    QObject::connect(s, SIGNAL(contragent_returned(int, QString)), SLOT(set_reserve_contragent(int,QString)));
    QObject::connect(s, SIGNAL(clear_reserve_contragent_signal()), SLOT(clear_reserve_contragent()));

    return s;
}

void CustomTabWidget::save_searcher_width(int mode, int index, int width){
    Searcher *s;
    for(int i = 0; i < this->count(); i++){
        s = static_cast<Searcher*>(this->widget(i));
        if(mode == 0) s->restore_white_width(index, width);
        else if(mode == 1) s->restore_grey_width(index, width);
        else if(mode == 2) s->restore_manager_reserve_width(index, width);
    }
}

void CustomTabWidget::save_searcher_order(int mode, int logical, int newvisual){
    Searcher *s;
    for(int i = 0; i < this->count(); i++){
        s = static_cast<Searcher*>(this->widget(i));
        if(mode == 0) s->restore_white_order(logical, newvisual);
        else if(mode == 1) s->restore_grey_order(logical, newvisual);
        else if(mode == 2) s->restore_manager_reserve_order(logical, newvisual);
    }
}

void CustomTabWidget::save_width(int index, int width, QString section){
    emit save_width_signal(index, width, section);
    if(section == "WHITE_COLUMNS")
        save_searcher_width(0, index, width);
    else if(section == "GREY_COLUMNS")
        save_searcher_width(1, index, width);
    else if(section == "MANAGER_RESERVE")
        save_searcher_width(2, index, width);
}

void CustomTabWidget::save_order(int logical, int newvisual, QString section){
    emit save_order_signal(logical, newvisual, section);
    if(section == "WHITE_COLUMNS")
        save_searcher_order(0, logical, newvisual);
    else if(section == "GREY_COLUMNS")
        save_searcher_order(1, logical, newvisual);
    else if(section == "MANAGER_RESERVE")
        save_searcher_order(2, logical, newvisual);
}

void CustomTabWidget::switch_reserves(){
    searcher_show_reserve = !searcher_show_reserve;
    //qDebug() << searcher_show_reserve;
    Searcher *s;
    for(int i = 0; i < this->count(); i++){
        s = static_cast<Searcher*>(this->widget(i));
        s->switch_reserves();
    }
}

void CustomTabWidget::refresh_white_searcher(){
    Searcher *s;
    for(int i = 0; i < this->count(); i++){
        s = static_cast<Searcher*>(this->widget(i));
        s->refresh_white_table();
    }
}

void CustomTabWidget::refresh_grey_searcher(){
    Searcher *s;
    for(int i = 0; i < this->count(); i++){
        s = static_cast<Searcher*>(this->widget(i));
        s->refresh_grey_table();
    }
}

void CustomTabWidget::refresh_blue_searcher(){
    Searcher *s;
    for(int i = 0; i < this->count(); i++){
        s = static_cast<Searcher*>(this->widget(i));
        s->refresh_blue_table();
    }
}

void CustomTabWidget::refresh_searcher(){
    refresh_white_searcher();
    refresh_grey_searcher();
    refresh_blue_searcher();
}

void CustomTabWidget::set_reserve_contragent(int id, QString name){
    Searcher *s;
    for(int i = 0; i < this->count(); i++){
        s = static_cast<Searcher*>(this->widget(i));
        s->set_reserve_contragent(id, name);
    }
}

void CustomTabWidget::clear_reserve_contragent(){
    Searcher *s;
    for(int i = 0; i < this->count(); i++){
        s = static_cast<Searcher*>(this->widget(i));
        s->clear_reserve_contragent();
    }
}

void CustomTabWidget::switch_hidden(){
    Searcher *s;
    for(int i = 0; i < this->count(); i++){
        s = static_cast<Searcher*>(this->widget(i));
        s->switch_hidden();
    }
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
