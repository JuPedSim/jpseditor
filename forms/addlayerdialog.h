#ifndef ADDLAYERDIALOG_H
#define ADDLAYERDIALOG_H

#include <QDialog>

namespace Ui {
class AddLayerDialog;
}

class AddLayerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddLayerDialog(QWidget *parent = nullptr);
    ~AddLayerDialog();

private slots:
    void on_cancelButton_clicked();

    void on_confirmButton_clicked();

private:
    Ui::AddLayerDialog *ui;
};

#endif // ADDLAYERDIALOG_H
