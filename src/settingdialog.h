#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QList>
#include <QMap>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

    void setCurrentSetting(QMap<QString, QString> settingsmap);

signals:
    void sendSetting(QMap<QString, QString>);

private slots:
    void on_cancelButton_clicked();

    void on_chooseButton_clicked();

    void on_confirmButton_clicked();

    void on_choosejpsButton_clicked();

    void on_jpsvisButton_clicked();

    void on_cancelButton_page2_clicked();

    void on_confirmButton_page2_clicked();

    void on_applyButton_clicked();

private:
    Ui::SettingDialog *ui;
};

#endif // SETTINGDIALOG_H
