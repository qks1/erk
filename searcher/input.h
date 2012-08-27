#ifndef INPUT_H
#define INPUT_H

#include <QtGui>
#include "constants.h"

class Input : public QWidget
{
    Q_OBJECT
public:
    explicit Input(QWidget *parent = 0);

    void delete_last_symbol();
    int id_combobox_value();



private:
    // три радиокнопки, выбирающие режим поиска
    QRadioButton *id_mode;
    QRadioButton *begin_mode;
    QRadioButton *parts_mode;

    // текстовое поле
    QLineEdit *text;

    // кнопка сброса
    QPushButton *reset;

    // две радиокнопки, управляющие режимом поиска по id (== или >=).
    // появляются только когда выбран режим id_mode
    QRadioButton *eq;
    QRadioButton *ge;
    QComboBox *eqge;

    inline void set_layout();
    inline void connects();

    
signals:
    void text_changed_signal(int, QString);
    void reset_signal();
    
public slots:
    void text_changed_slot(QString);
    void id_mode_changed_slot(int);
    void set_text(QString);
    void reset_text();
    void send_reset();

private slots:
    void hide_eqge();
    void show_eqge();
    void set_cursor_to_begin();
    void set_validator();
    void remove_validator();

};

#endif // INPUT_H
