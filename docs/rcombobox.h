#ifndef RCOMBOBOX_H
#define RCOMBOBOX_H

#include <QtGui>
#include "common/customcombobox.h"
#include "common/helpers.h"
#include "common/constants.h"


/*
    В р-комбобоксах автозавершение нужно по любому фрагменту строки, а не только по началу.
    Для этого реализован весьма продвинутый комплитер и ещё более продвинутый комбобокс.
    Все пояснения в файле исходников.
*/

class AdvancedCompleter : public QCompleter
{
    Q_OBJECT

public:
    AdvancedCompleter(/*QStringList *strings_list, QStringList *data_list, */QObject *parent = 0);
    void update(QString words);
    void set_strings_list(QStringList lst);
    void set_data_list(QStringList lst);

private:
    QStringList strings_list, data_list;
    QStringListModel popup_model;
    int count;
};



class RComboBox : public CustomComboBox
{
    Q_OBJECT

public:
    RComboBox(QWidget *parent = 0);
    ~RComboBox();

    void setCompleter(AdvancedCompleter *c);
    AdvancedCompleter *current_completer() const;
    void set_timeout(int ms);
    void set_min_chars(int chars);
    bool first;

protected:
    void keyPressEvent(QKeyEvent *e);
    //void mousePressEvent(QMouseEvent *e);
    //bool eventFilter(QObject *obj, QEvent *e);
    //void showPopup();
    //void focusOutEvent(QFocusEvent *e);

private slots:
    void insertCompletion(const QString &completion);
    void update_popup();

public slots:
    void setEditText(const QString &text);

private:
    QTimer *timer;
    AdvancedCompleter *completer;
    int timeout;
    int min_chars;

};

#endif // RCOMBOBOX_H
