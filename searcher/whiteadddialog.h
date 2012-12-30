#ifndef WHITEADDDIALOG_H
#define WHITEADDDIALOG_H

#include <QDialog>
#include <QtGui>
#include "../common/customcombobox.h"
#include "../common/helpers.h"
#include "../common/constants.h"
#include "../common/headercomboboxdelegate.h"

namespace Ui {
class whiteAddDialog;
}

class WhiteAddDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit WhiteAddDialog(int gr, QString name, int mode, QWidget *parent = 0);
    WhiteAddDialog(int id, int mode, QWidget *parent = 0);
    ~WhiteAddDialog();
    
private:
    Ui::whiteAddDialog *ui;
    QTreeWidget *wgt;
    QList<QWidget*> unrequired_widgets;
    QList<DetailSmall> details_names;
    QStringList analogs_old;
    QMap<QString, QString> params;
    int dialog_mode;
    bool params_enabled;
    QString valid_error_text, success_text;

    inline void create_unrequired_list();
    bool fill_groups_box();
    bool fill_subgroup(int);
    void change_params_labels(int);
    void fill_names_box(int);
    bool fill_weight_box();
    bool fill_unit_box();
    void disable_params();
    bool save_new();
    bool save_edited();
    bool fill_current();
    inline void connects();

    // функции валидации
    bool valid();
    bool valid_name();
    bool valid_weight();
    bool valid_photo();

    bool add_analogs();
    bool proceed_photo();

private slots:
    void change_group(int);
    void enable_params(QString);
    void open_photo_dialog();
    void clear_photo_path();
    void add_analog_to_list();
    void remove_analog_from_list();

    bool apply();
    void accept();
};

#endif // WHITEADDDIALOG_H
