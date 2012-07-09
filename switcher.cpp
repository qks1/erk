#include "switcher.h"
#include "constants.h"
#include <QtGui>

//--------------------------------------------------------------------------//
//--------------------------- КОНСТРУКТОР-----------------------------------//
//--------------------------------------------------------------------------//

Switcher::Switcher(QWidget *parent) :
    QWidget(parent)
{
    // СОЗДАЁМ ОБЪЕКТЫ:
    // layout
    switcher_layout = new QHBoxLayout;

    // элементы для постраничного режима
    first = new QPushButton();
    prev = new QPushButton();
    current_page = new QLineEdit();
    total_pages = new QLabel();
    next = new QPushButton();
    last = new QPushButton();
    items_on_page = new QLineEdit();
    onpage_label = new QLabel("на странице / ");
    show_all = new QPushButton();

    // элементы для одоностраничного режима
    total_items = new QLabel();
    switch_to_multipages = new QPushButton("Постранично");
    // для надписи onpage_label и кнопки switch_to_multipages устанавливаем текст прямо в конструкторе, т.к. меняться он не будет

    // режим по умолчанию - постраничный
    mode = MULTIPAGE_MODE;

    // размеры по умолчанию (что есть что - см. в хедере)
    // размеры элементов, содержащих текст, устанавливаем относительно размера текста (подгоняем на глазок)
    button_size = 16;
    total_label_size = SMALL_FONT_SIZE*4;
    line_edit_size = SMALL_FONT_SIZE*3;
    big_button_size = SMALL_FONT_SIZE*7;

    // иконки для кнопок переключения страниц
    QPixmap first_icon("images/first.png");
    QPixmap prev_icon("images/prev.png");
    QPixmap next_icon("images/next.png");
    QPixmap last_icon("images/last.png");

    // устанавливаем их
    first->setIcon(first_icon);
    prev->setIcon(prev_icon);
    next->setIcon(next_icon);
    last->setIcon(last_icon);

    // формируем и устанавливаем шрифт, используемый в переключателе
    set_fonts();

    // устанавливаем размеры элементов
    set_sizes();

    // размещаем элементы
    set_layout();

    // устанавливаем непрозрачный фон
    set_background();

    // соединяем сигналы со слотами
    connects();
}

//--------------------------------------------------------------------------//

inline void Switcher::connects(){
    // при нажатии на кнопку переключения страницы - послать сигнал "page_changed"
    QObject::connect(this->first, SIGNAL(clicked()),
                     this, SLOT(first_clicked()));
    QObject::connect(this->prev, SIGNAL(clicked()),
                     this, SLOT(prev_clicked()));
    QObject::connect(this->next, SIGNAL(clicked()),
                     this, SLOT(next_clicked()));
    QObject::connect(this->last, SIGNAL(clicked()),
                     this, SLOT(last_clicked()));

    // при вводе номера страницы - послать сигнал "page_changed"
    QObject::connect(this->current_page, SIGNAL(returnPressed()),
                     this, SLOT(page_entered()));

    // при вводе кол-ва элементов на странице - послать сигнал "items_on_page_changed"
    QObject::connect(this->items_on_page, SIGNAL(returnPressed()),
                     this, SLOT(items_on_page_entered()));

    // при нажатии на кнопку "все на одной странице" - послать сигнал "singlepage_mode_selected"
    QObject::connect(this->show_all, SIGNAL(clicked()),
                     this, SLOT(singlepage_mode_clicked()));

    // при нажатии на кнопку "постранично" - послать сигнал "multipage_mode_selected"
    QObject::connect(this->switch_to_multipages, SIGNAL(clicked()),
                     this, SLOT(multipage_mode_clicked()));


}

//--------------------------------------------------------------------------//

inline void Switcher::set_background(){
    // создаём и устанавливаем белый непрозрачный фон
    QPalette p;
    p.setColor(this->backgroundRole(), Qt::white);
    this->setPalette(p);
    // заставляем его появляться автоматически
    this->setAutoFillBackground(true);
}

//--------------------------------------------------------------------------//

inline void Switcher::set_sizes(){
    // у всех элементов одинаковая высота, равная button_size. Ширина же бывает разная.
    // квадратные кнопки переключения страниц:
    first->setFixedSize(button_size, button_size);
    prev->setFixedSize(button_size, button_size);
    next->setFixedSize(button_size, button_size);
    last->setFixedSize(button_size, button_size);

    // текстовые поля:
    current_page->setFixedSize(line_edit_size, button_size);
    items_on_page->setFixedSize(line_edit_size, button_size);

    // надпись, содержащая общее кол-во страниц
    total_pages->setFixedWidth(total_label_size);

    // для надписи "на странице" текст никогда не меняется, поэтому подгоним ширину надписи под текст да так и оставим
    onpage_label->adjustSize();
    onpage_label->setFixedWidth(onpage_label->width());

    // длинные кнопки переключения режима
    show_all->setFixedSize(big_button_size, button_size);
    switch_to_multipages->setFixedSize(big_button_size, button_size);

    // надпись ".. элементов". Установим для неё такую ширину, как для двух вышеупомянутых кнопок.
    total_items->setFixedSize(big_button_size, button_size);
}

//--------------------------------------------------------------------------//

inline void Switcher::set_fonts(){
    // в переключателе должен использоваться шрифт меньший, чем в остальной программе. Размер его задан в constants.cpp
    QFont lineFont;
    lineFont.setPixelSize(SMALL_FONT_SIZE);

    // установим шрифт на элементы: кнопки (кроме тех, что с иконками), текстовые поля, лейблы
    current_page->setFont(lineFont);
    items_on_page->setFont(lineFont);
    total_pages->setFont(lineFont);
    onpage_label->setFont(lineFont);
    show_all->setFont(lineFont);
    total_items->setFont(lineFont);
    switch_to_multipages->setFont(lineFont);
}


//--------------------------------------------------------------------------//
//--------------------------- ФУНКЦИИ --------------------------------------//
//--------------------------------------------------------------------------//

void Switcher::clear_layout(){
    // при смене режима отображения меняются элементы переключателя,
    // соответственно, меняется layout. Поэтому надо удалить все элементы предыдущего layout,
    // удалить сам layout и создать новый

 /* QLayoutItem *item;
    while((item = switcher_layout->takeAt(0)) != 0){
        switcher_layout->removeItem(item);
        item->widget()->hide();
    }
 */

    // Естественно, надо бы использовать закомментированную часть.
    // Но она почему-то работает не всегда корректно, посему да, мы не ищем лёгких путей.
    // В зависимости от режима, удаляем из layout все элементы поочерёдно.
    // А чтобы они более не вылезали, каждый надо скрыть.

    if(this->mode == MULTIPAGE_MODE){
        switcher_layout->removeWidget(first);
        switcher_layout->removeWidget(prev);
        switcher_layout->removeWidget(current_page);
        switcher_layout->removeWidget(total_pages);
        switcher_layout->removeWidget(next);
        switcher_layout->removeWidget(last);
        switcher_layout->removeWidget(items_on_page);
        switcher_layout->removeWidget(onpage_label);
        switcher_layout->removeWidget(show_all);

        first->hide();
        prev->hide();
        current_page->hide();
        total_pages->hide();
        next->hide();
        last->hide();
        items_on_page->hide();
        onpage_label->hide();
        show_all->hide();
    }
    else if(this->mode = SINGLEPAGE_MODE){
        switcher_layout->removeWidget(total_items);
        switcher_layout->removeWidget(switch_to_multipages);

        total_items->hide();
        switch_to_multipages->hide();
    }

    // теперь наконец удалим сам layout
    delete switcher_layout;

    // а за формирование нового layout отвечают уже другие функции.
}

//--------------------------------------------------------------------------//

inline void Switcher::set_layout(){
    // формирование нового layout.
    // чтобы переключатель нормально отображался в таблице,
    // установим в ноль отступ его от других виджетов и расстояние между виджетами в нём.
    switcher_layout->setMargin(0);
    switcher_layout->setSpacing(0);

    // в зависимости от режима, добавляем в layout необходимые элементы
    if(mode == MULTIPAGE_MODE){
        switcher_layout->addWidget(first);
        switcher_layout->addWidget(prev);
        switcher_layout->addWidget(current_page);
        switcher_layout->addWidget(total_pages);
        switcher_layout->addWidget(next);
        switcher_layout->addWidget(last);
        //switcher_layout->addSpacing(spacing);
        switcher_layout->addWidget(items_on_page);
        switcher_layout->addWidget(onpage_label);
        switcher_layout->addWidget(show_all);

        // устанавливаем размер переключателя, считающийся как сумма размеров всех виджетов.
        this->setFixedSize(button_size*4+line_edit_size*2+total_label_size+onpage_label->width()+big_button_size, button_size);
    }
    else if(mode == SINGLEPAGE_MODE){
        switcher_layout->addWidget(total_items);
        switcher_layout->addWidget(switch_to_multipages);

        // размер считается аналогично.
        this->setFixedSize(big_button_size*2, button_size);

    }

    // устанавливаем layout на наш переключатель
    this->setLayout(switcher_layout);

    // если функция вызывается не из конструктора, т.е. layout формируется не первично,
    // для каждого виджета необходимо вызывать show(), чтобы он отобразился.
    // Но т.к. функция может вызываться из конструктора, show() будем вызывать в других функциях.
}

//--------------------------------------------------------------------------//

void Switcher::switch_to_singlepage_mode(){
    // вызывается при нажатии кнопки "Все на одной странице"
    // на всякий случай надо проверить, находимся ли мы в мультистраничном режиме.
    if(this->mode == MULTIPAGE_MODE){
        // необходимо удалить с экрана все элементы переключателя и установить новые.
        // удаляем все виджеты из layout и сам layout
        this->clear_layout();

        // меняем режим переключателя (раньше этого сделать было нельзя, чтобы корректно отработала функция clear_layout())
        this->mode = SINGLEPAGE_MODE;

        // создаём новый layout
        switcher_layout = new QHBoxLayout;

        // добавляем в него виджеты
        this->set_layout();

        // показываем на экране каждый виджет
        total_items->show();
        switch_to_multipages->show();
    }
}

//--------------------------------------------------------------------------//

void Switcher::switch_to_multipage_mode(){
    // функция работает аналогично switch_to_singlepage_mode()
    if(this->mode == SINGLEPAGE_MODE){
        // очищаем и удаляем layout
        this->clear_layout();

        // меняем режим
        this->mode = MULTIPAGE_MODE;

        // создаём новый layout
        switcher_layout = new QHBoxLayout;
        this->set_layout();

        // показываем все виждеты
        first->show();
        prev->show();
        current_page->show();
        total_pages->show();
        next->show();
        last->show();
        items_on_page->show();
        onpage_label->show();
        show_all->show();
    }
}




//--------------------------------------------------------------------------//
//---------- ПУБЛИЧНЫЕ ФУНКЦИИ, УСТАНАВЛИВАЮЩИЕ ТЕКСТ В ВИДЖЕТЫ ------------//
//--------------------------------------------------------------------------//

void Switcher::set_current_page(int page){
    // устанавливаем номер страницы в текстовое поле current_page
    current_page->setText(QString::number(page));
}

void Switcher::set_total_pages(int pages){
    // устанавливаем кол-во страниц в надпись total_pages
    total_pages->setText("из " + QString::number(pages));
}

void Switcher::set_items_on_page(int iop){
    // устанавливаем кол-во элементов на странице в текстовое поле items_on_page
    items_on_page->setText(QString::number(iop));
}


void Switcher::set_total_items_label(int items){
    // устанавливаем кол-во элементов в надись total_items
    total_items->setText("Позиций: " + QString::number(items));
}

void Switcher::set_show_all(int items){
    // устанавливаем кол-во элементов на кнопку переключения в одностраничный режим
    show_all->setText("Все " + QString::number(items));
}


//--------------------------------------------------------------------------//
//----------- ПУБЛИЧНЫЕ ФУНКЦИИ, ВОЗВРАЩАЮЩИЕ ТЕКСТ ИЗ ВИДЖЕТОВ ------------//
//--------------------------------------------------------------------------//

int Switcher::page_number(){
    // возвращаем номер страницы из поля current_page
    return current_page->text().toInt();
}



//--------------------------------------------------------------------------//
//--------------------------- СЛОТЫ ----------------------------------------//
//--------------------------------------------------------------------------//

// Нижеследующие слоты просто высылают сигналы, перекладывая всю работу на родительскую таблицу

void Switcher::first_clicked(){
    // если нажата кнопка "на первую страницу"
    emit page_changed(PAGE_FIRST);
}

//--------------------------------------------------------------------------//

void Switcher::prev_clicked(){
    // если нажата кнопка "на предыдущую страницу"
    emit page_changed(PAGE_PREV);
}

//--------------------------------------------------------------------------//

void Switcher::next_clicked(){
    // если нажата кнопка "на следующую страницу"
    emit page_changed(PAGE_NEXT);
}

//--------------------------------------------------------------------------//

void Switcher::last_clicked(){
    // если нажата кнопка "на последнюю страницу"
    emit page_changed(PAGE_LAST);
}

//--------------------------------------------------------------------------//

void Switcher::page_entered(){
    // если введён номер страницы
    emit(page_changed(PAGE_NUMBER));
}

//--------------------------------------------------------------------------//

void Switcher::items_on_page_entered(){
    // если введено кол-во элементов на странице
    emit items_on_page_changed(items_on_page->text().toInt());
}

//--------------------------------------------------------------------------//

void Switcher::singlepage_mode_clicked(){
    // если выбран переход в одностраничный режим
    emit singlepage_mode_selected();
}

//--------------------------------------------------------------------------//

void Switcher::multipage_mode_clicked(){
    // если выбран переход в мультистраничный режим
    emit multipage_mode_selected();
}

//--------------------------------------------------------------------------//
