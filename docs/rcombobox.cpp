#include "rcombobox.h"

/*
    В конструктор комплитера передаётся два списка строк,
    содержащих, в общем-то, идентичную информацию.
    Только в первом списке содержится красивая строка для вывода на экран,
    со всеми пробелами, запятыми, скобочками, доп. словами типа г., тел. и т.п.
    А во втором - строка для поиска, из которой убраны все лишние слова
    и большая часть знаков препинания и пробелов.
    Из знаков препинания могут быть только запятые, разделяющие номера телефонов, если их несколько.

    В классе содержатся:
      - main_model - указатель на упомянутую двухстолбцовую модель. Передаётся в конструкторе.
      - popup_model - строковая модель для всплывающего окна комплитера.
        Обновляется каждый раз при обновлении текста.
    При формировании этой модели вызывается функция update(). Подробнее - в самом комбобоксе.
*/

AdvancedCompleter::AdvancedCompleter(/*QStringList *strings_list, QStringList *data_list, */QObject *parent) :
        QCompleter(parent)
{
    this->strings_list = QStringList();
    this->data_list = QStringList();
    this->setModel(&popup_model);

    //connect(main_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(update_lists()));

}

void AdvancedCompleter::update(QString words)
{
    /* Строку, введённую в комбобокс, нужно обработать и разбить на слова (если их несколько).
       Тяжелее всего с номерами телефонов.
       Они могут быть записаны как угодно: сплошным текстом, через дефисы, через пробелы,
       со скобками для кода и без. Поэтому если в тексте мы видим цифры,
       разделённые пробелами или знаками препинания - удаляем этот разделитель,
       склеивая цифры в сплошной текст. Однако, в тексте может быть несколько номеров подряд
       (предполагается, что они разделены запятыми), поэтому запятые между цифрами не удаляем.
       В результате, например, текст "8-(964)-123-45-67, 8 495 987 654 1"
       превратится в 89641234567,84959876541.
       В строке для поиска номера записаны именно таким сплошным текстом.
    */
    QRegExp pattern = QRegExp("(\\d)([^a-zA-Zа-яА-Я0-9,;]+)(\\d)");
    QStringList source_words = words.replace(pattern, "\\1\\3")
            .replace(QRegExp("[,;]"), " ")
            .remove(QRegExp("[^a-zA-Zа-яА-Я0-9 ]"))
            .split(" ", QString::SkipEmptyParts);
    QStringList filtered;
    QString cur;
    bool flag;
    int maxwidth = 0;
    for(int i = 0; i < strings_list.count(); i++){
        flag = true;
        foreach(cur, source_words){
            if(!data_list.at(i).contains(cur, Qt::CaseInsensitive)){
                flag = false;
                break;
            }
        }
        if(flag)
            filtered << strings_list.at(i);
    }
    QFontMetrics fm(popup()->fontMetrics());
    for(int i = 0; i < filtered.count(); i++)
        maxwidth = qMax(fm.width(filtered.at(i)), maxwidth);

    popup()->setMinimumWidth(maxwidth + 10);
    popup_model.setStringList(filtered);

    complete();

}

void AdvancedCompleter::set_strings_list(QStringList lst){
    this->strings_list = lst;
}

void AdvancedCompleter::set_data_list(QStringList lst){
    this->data_list = lst;
}



RComboBox::RComboBox(QWidget *parent)
    : CustomComboBox(parent)
{
    this->setEditable(true);
    this->setInsertPolicy(QComboBox::NoInsert);
    completer = 0;
    timer = new QTimer(this);
    timer->setSingleShot(true);
    timeout = 1000;
    min_chars = 3;
    first = false;
    //this->lineEdit()->installEventFilter(this);
    connect(timer, SIGNAL(timeout()), SLOT(update_popup()));
}

RComboBox::~RComboBox()
{
}

void RComboBox::setCompleter(AdvancedCompleter *c)
{
    if (this->completer)
        QObject::disconnect(this->completer, 0, this, 0);

    this->completer = c;

    if (!this->completer)
        return;

    this->completer->setWidget(this);
    connect(this->completer, SIGNAL(activated(const QString&)), this, SLOT(insertCompletion(const QString&)));
}

AdvancedCompleter *RComboBox::current_completer() const
{
    return completer;
}

void RComboBox::insertCompletion(const QString& completion)
{
    setEditText(completion);
    //selectAll();
}


void RComboBox::keyPressEvent(QKeyEvent *e)
{
    timer->stop();
    if (completer && completer->popup()->isVisible())
    {
        // The following keys are forwarded by the completer to the widget
        switch (e->key())
        {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return; // Let the completer do default behavior
        }
    }

    if (e->text().length() > 0 && e->text().at(0).isPrint()){
        if(this->lineEdit()->selectedText().size() > 0)
            this->lineEdit()->del();
        QString cur = currentText();
        int pos = this->lineEdit()->cursorPosition();
        cur = cur.left(pos)
                .append(e->text())
                .append(cur.right(cur.length()-pos));
        setEditText(cur);
        this->lineEdit()->setCursorPosition(pos+1);
        //CustomComboBox::keyPressEvent(e);
    }
    else CustomComboBox::keyPressEvent(e);

    if(currentText().length() < min_chars && completer->popup()->isVisible())
        completer->popup()->close();
    this->lineEdit()->deselect();

    timer->start(timeout);
}


void RComboBox::update_popup(){
    if (completer && currentText().length() >= min_chars){
        completer->update(currentText());
        completer->popup()->setCurrentIndex(completer->completionModel()->index(0, 0));
    }
}

void RComboBox::set_timeout(int ms){
    this->timeout = ms;
}

void RComboBox::set_min_chars(int chars){
    this->min_chars = chars;
}

void RComboBox::setEditText(const QString &text){
    CustomComboBox::setEditText(text);
    lineEdit()->setCursorPosition(0);
}

/*
void RComboBox::mousePressEvent(QMouseEvent *e){
    first = false;
    CustomComboBox::mousePressEvent(e);
    //this->lineEdit()->selectAll();
}

bool RComboBox::eventFilter(QObject *obj, QEvent *e){
    if(obj == this->lineEdit() && e->type() == QEvent::MouseButtonPress){
        first = false;
        if(lineEdit()->selectedText().length() == 0 && first){
            this->lineEdit()->selectAll();
            return true;
        }
    }
    if(obj == this->lineEdit() && e->type() == QEvent::FocusOut){
        first = true;
        lineEdit()->deselect();
        return true;
    }

    return QComboBox::eventFilter(obj, e);
}

void RComboBox::showPopup(){
    CustomComboBox::showPopup();
}

void RComboBox::focusOutEvent(QFocusEvent *e){
    first = true;
    CustomComboBox::focusOutEvent(e);
}
*/
