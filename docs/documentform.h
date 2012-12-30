#ifndef DOCUMENTFORM_H
#define DOCUMENTFORM_H

#include <QDialog>
#include <QStringList>
#include "lists/contragentslist.h"
#include "common/constants.h"
#include "common/helpers.h"
#include "common/customcombobox.h"
#include "r_classes.h"
#include "rcombobox.h"

class TwoColorsSqlModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    TwoColorsSqlModel(QWidget *parent = 0);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
};




namespace Ui {
class DocumentForm;
}

class DocumentForm : public QDialog
{
    Q_OBJECT
    
public:
    explicit DocumentForm(QStackedWidget *stack,
                          QStack<QWidget*> prev_wgts,
                          int doctype,
                          int provider_id,
                          QString provider,
                          int contragent_id,
                          QString contragent,
                          int consignee_id,
                          QString consignee,
                          QString notes,
                          QString r_notes,
                          bool print_r,
                          QWidget *parent = 0);
    ~DocumentForm();
    Ui::DocumentForm *ui;
    // скопировать из конструктора и сохранить
    int doctype;
    int old_provider_id, provider_id;
    int old_contragent_id, contragent_id;
    int old_consignee_id, consignee_id;
    QString old_notes, notes, old_r_notes, r_notes;
    AdvancedCompleter *r_completer;
    TwoColorsSqlModel *r_model;
    void fill_r_box();
    QString r_string();

signals:
    void something_changed();


private:
    QStackedWidget *stack;
    QStack<QWidget*> prev_wgts;
    inline void connects();

public slots:
    void r_notes_changed(QString);

private slots:
    void select_provider();
    void select_contragent();
    void select_consignee();
    void set_provider(int, QString);
    void set_contragent(int, QString);
    void set_consignee(int, QString);
    void delete_provider();
    void delete_contragent();
    void delete_consignee();
    void close_current();
    void notes_changed(QString);
    void add_rstring();
    void add_to_rbox(QString);

};



#endif // DOCUMENTFORM_H
