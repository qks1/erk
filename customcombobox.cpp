#include "customcombobox.h"

CustomComboBox::CustomComboBox(QWidget *parent) :
    QComboBox(parent)
{
    this->setEditable(true);
    this->setAutoCompletion(true);
    this->setInsertPolicy(QComboBox::NoInsert);
}

void CustomComboBox::mousePressEvent(QMouseEvent *e){
    if(e->button() == Qt::LeftButton){
        this->showPopup();
        //QComboBox::showPopup();
        emit(fill_box_signal());
    }
    QComboBox::mousePressEvent(e);
}

void CustomComboBox::focusInEvent(QFocusEvent *e){
    //if(this->currentText() == ANY_ITEM_TEXT)
        //this->clearEditText();
    //emit fill_box_signal();
    //QComboBox::showPopup();
    QComboBox::focusInEvent(e);
}

void CustomComboBox::closeEvent(QCloseEvent *){

}
