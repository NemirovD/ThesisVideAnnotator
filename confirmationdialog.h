#ifndef CONFIRMATIONDIALOG_H
#define CONFIRMATIONDIALOG_H

#include <QDialog>

namespace Ui {
class ConfirmationDialog;
}

class ConfirmationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmationDialog(QWidget *parent = 0);
    ConfirmationDialog(QString text, QWidget * parent = 0);
    ~ConfirmationDialog();
signals:
    void confirmed(bool);
public slots:
    void onContinueClick();
    void onCancelClick();
private:
    Ui::ConfirmationDialog *ui;
};

#endif // CONFIRMATIONDIALOG_H
