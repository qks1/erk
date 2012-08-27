#include "customcombobox.h"

CustomComboBox::CustomComboBox(QWidget *parent) :
    QComboBox(parent)
{
    this->setEditable(true);
    this->setAutoCompletion(true);
    this->setInsertPolicy(QComboBox::NoInsert);
}

void CustomComboBox::mousePressEvent(QMouseEvent *e){
    if(e->button() == Qt::LeftButton)
        emit(fill_boxes_signal());
    QComboBox::mousePressEvent(e);
}

void CustomComboBox::focusInEvent(QFocusEvent *e){
    //if(this->currentText() == ANY_ITEM_TEXT)
        //this->clearEditText();
}

void CustomComboBox::closeEvent(QCloseEvent *){

}
