#include "movepositions.h"

MovePositions::MovePositions(QStandardItemModel *list_model, QWidget *parent) : QDialog(parent){
    setWindowTitle("Переместить позиции");
    setAttribute(Qt::WA_DeleteOnClose);
    reserves_list_model = list_model;
    create_widgets();
    set_layout();

    max_docs = 10;
    docs_button->setChecked(true);
    form_reserves_list();

    switch_widget();

    connects();
}

inline void MovePositions::create_widgets(){
    docs_button = new QRadioButton("Документы");
    reserves_button = new QRadioButton("Заявки");
    ok_button = new QPushButton("Переместить");
    cancel_button = new QPushButton("Отмена");
    docs_list = new DocsList(false, 0, QStack<QWidget*>(), reserves_list_model, this);
    reserves_list = new QListView();
    reserves_list->setModel(reserves_list_model);
}

inline void MovePositions::set_layout(){
    // лайаут с двумя радиокнопками, они будут расположены вертикально и сдвинуты кверху
    QVBoxLayout *radiobuttons_vlt = new QVBoxLayout();
    radiobuttons_vlt->addWidget(docs_button);
    radiobuttons_vlt->addWidget(reserves_button);
    radiobuttons_vlt->addStretch(1);

    // лайаут с кнопками ОК и Отмена, они будут в самом низу справа
    QHBoxLayout *buttons_hlt = new QHBoxLayout();
    buttons_hlt->addStretch(1);
    buttons_hlt->addWidget(ok_button);
    buttons_hlt->addWidget(cancel_button);

    // формируем stacked_layout
    layout = new QStackedLayout();
    layout->addWidget(reserves_list);
    layout->addWidget(docs_list);

    // главный лайаут
    // состоит из двух: сверху hbox с радиокнопками и стакедлайаутом, снизу кнопки ок и отмена
    QVBoxLayout *main_lt = new QVBoxLayout();
    QHBoxLayout *top_lt = new QHBoxLayout();
    top_lt->addLayout(radiobuttons_vlt);
    top_lt->addLayout(layout);
    main_lt->addLayout(top_lt);
    main_lt->addLayout(buttons_hlt);

    this->setLayout(main_lt);
}

inline void MovePositions::connects(){
    // перекючение виджетов по радиокнопкам
    connect(this->reserves_button, SIGNAL(clicked()), SLOT(switch_widget()));
    connect(this->docs_button, SIGNAL(clicked()), SLOT(switch_widget()));
    connect(docs_list, SIGNAL(id_signal(int)), SLOT(doc_selected_slot(int)));
    connect(reserves_list, SIGNAL(doubleClicked(QModelIndex)), SLOT(reserve_selected_slot(QModelIndex)));
    connect(ok_button, SIGNAL(clicked()), SLOT(accept()));
    connect(cancel_button, SIGNAL(clicked()), SLOT(reject()));
}

inline void MovePositions::form_reserves_list(){
    // в таблице будут два столбца - "Заявка №" и "Для кого"
    // к каждой ячейке с номером заявки присовокупляем оный в UserRole

}


////// СЛОТЫ
void MovePositions::switch_widget(){
    if(this->reserves_button->isChecked())
        layout->setCurrentWidget(reserves_list);
    else if(this->docs_button->isChecked())
        layout->setCurrentWidget(docs_list);
}

void MovePositions::doc_selected_slot(int id){
    emit doc_selected(id);
    QDialog::accept();
}

void MovePositions::reserve_selected_slot(QModelIndex index){
    emit reserve_selected(index.row());
    QDialog::accept();
}

void MovePositions::accept(){
    if(docs_button->isChecked() && docs_list->current_id() > 0){
        emit doc_selected(docs_list->current_id());
    }
    else if(reserves_button->isChecked()){
        emit reserve_selected(reserves_list->currentIndex().row());
    }
    QDialog::accept();
}
