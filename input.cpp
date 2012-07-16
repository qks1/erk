#include "input.h"

Input::Input(QWidget *parent) :
    QWidget(parent)
{
    // создаём объекты
    id_mode = new QRadioButton("id");
    begin_mode = new QRadioButton("сначала");
    parts_mode = new QRadioButton("по фрагменту");
    begin_mode->setChecked(true);

    text = new QLineEdit();
    reset = new QPushButton("X");

    eqge = new QComboBox();
    eqge->addItem(">");
    eqge->addItem("=");

    QFont lineFont;
    lineFont.setPixelSize(11);
    id_mode->setFont(lineFont);
    begin_mode->setFont(lineFont);
    parts_mode->setFont(lineFont);

    // размещаем
    set_layout();

    connects();
}

inline void Input::connects(){
    // когда изменяется текст, высылаем сигнал поисковику
    QObject::connect(this->text, SIGNAL(textChanged(QString)),
                     this, SLOT(text_changed_slot(QString)));

    // когда происходит клик на радиокнопку, очищать текст в поле и текстовые фильтры
    QObject::connect(this->id_mode, SIGNAL(clicked()),
                     this, SLOT(send_reset()));
    QObject::connect(this->begin_mode, SIGNAL(clicked()),
                     this, SLOT(send_reset()));
    QObject::connect(this->parts_mode, SIGNAL(clicked()),
                     this, SLOT(send_reset()));
    QObject::connect(this->reset, SIGNAL(clicked()),
                     this, SLOT(send_reset()));

    QObject::connect(this->id_mode, SIGNAL(clicked()),
                     this, SLOT(show_eqge()));
    QObject::connect(this->begin_mode, SIGNAL(clicked()),
                     this, SLOT(hide_eqge()));
    QObject::connect(this->parts_mode, SIGNAL(clicked()),
                     this, SLOT(hide_eqge()));

    QObject::connect(this->eqge, SIGNAL(activated(int)),
                     this, SLOT(id_mode_changed_slot(int)));
}

inline void Input::set_layout(){
    text->setMaximumWidth(150);
    reset->setFixedSize(25,25);
    eqge->setFixedWidth(45);

    // три кнопки размещаем вертикально
    // к первой (id) присовокупляем комбобокс с выбором ">=" или "=="
    QHBoxLayout *idlt = new QHBoxLayout;
    idlt->setSpacing(0);
    idlt->setMargin(0);
    idlt->addWidget(id_mode);
    idlt->addWidget(eqge);

    QVBoxLayout *radio = new QVBoxLayout;
    radio->addLayout(idlt);
    radio->addWidget(begin_mode);
    radio->addWidget(parts_mode);

    // основной layout
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setSpacing(0);
    layout->addLayout(radio);
    layout->addWidget(text);
    layout->addWidget(reset);

    hide_eqge();

    this->setLayout(layout);
}

int Input::id_combobox_value(){
    return this->eqge->currentIndex();
}

void Input::delete_last_symbol(){
    text->setText(text->text().remove(text->text().size()-1, 1));
}

void Input::set_text(QString txt){
    text->setText(txt);
}

void Input::reset_text(){
    this->set_text("");
}

void Input::send_reset(){
    reset_text();
    emit(reset_signal());
}


void Input::hide_eqge(){
    eqge->hide();
}

void Input::show_eqge(){
    eqge->show();
}



void Input::text_changed_slot(QString txt){
    if(id_mode->isChecked())
        emit(text_changed_signal(ID_MODE, txt));
    else if(begin_mode->isChecked())
        emit(text_changed_signal(BEGIN_MODE, txt));
    else if(parts_mode->isChecked())
        emit(text_changed_signal(PARTS_MODE, txt));
    else
        emit(text_changed_signal(ERROR_MODE, txt));
}

void Input::id_mode_changed_slot(int value){
    if(id_mode->isChecked()){
        emit(text_changed_signal(ID_MODE, text->text()));
    }
}
