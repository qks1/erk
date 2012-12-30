#include "customcombobox.h"

CustomComboBox::CustomComboBox(QWidget *parent) :
    QComboBox(parent)
{
    this->setEditable(false);
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

void CustomComboBox::showPopup()
{
    QListView* lv = qobject_cast< QListView* >( view() );
    int width = 0;
    int len = 0;
    QFontMetrics fm( lv->fontMetrics() );
    for( int i = 0; i < count(); i++ )
    {
        QString t( itemText( i ) );
        if( len < t.length() )
        {
            len = t.length();
            width = qMax( fm.width( t ), width );
        }
    }
    view()->setMinimumWidth( width + 10 +
        (count() > maxVisibleItems() ?
            view()->verticalScrollBar()->sizeHint().width() : 0 ) );

    QComboBox::showPopup();
}
