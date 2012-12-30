#ifndef R_CLASSES_H
#define R_CLASSES_H

#include <QtGui>
#include <QtSql>
#include "common/helpers.h"
#include "common/constants.h"
#include "common/customcombobox.h"
#include "rcombobox.h"

class AddRString : public QDialog{
    Q_OBJECT
public:
    explicit AddRString(int type, QString old_string = QString(), QWidget *parent = 0);

private:
    QLabel *company_label, *manager_label, *courier1_label, *courier2_label, *courier3_label, *info_label;
    RComboBox *company_box, *manager_box, *courier1_box, *courier2_box, *courier3_box;
    AdvancedCompleter *company_completer, *manager_completer, *courier1_completer, *courier2_completer, *courier3_completer;
    QLineEdit *info_edit;
    QPushButton *ok_button, *cancel_button, *form_button, *del_button;
    QTextEdit *old_text;
    QList<RComboBox*> couriers_boxes;
    QList<AdvancedCompleter*> couriers_completers;
    int couriers_ids[3];
    int company_id, manager_id;


    QString old_string, new_string;
    bool editing;
    int type;

    void fill_companies();
    void fill_managers(int company = 0);
    void fill_couriers(int company = 0);
    void decompose();
    bool form_string();
    void add_to_rbox(QString, bool creation);

    inline void set_layout();

signals:
    void string_signal(QString);

private slots:
    void company_changed(int);
    void company_text_changed(QString);

    void form_string_slot();
    void del_string_slot();
    void accept();
};

#endif // R_CLASSES_H
