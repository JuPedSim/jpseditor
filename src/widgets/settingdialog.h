#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QList>
#include <QMap>
#include <QSettings>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

public slots:
    void loadSettings();
    void cancelButtonClicked();
    void chooseBackupButtonClicked();
    void applyButtonClicked();
    void chooseJPScoreButtonClicked();
    void chooseJPSvisButtonClicked();

signals:
    void changeTimer(int interval);

private:
    Ui::SettingDialog *ui;
};

#endif // SETTINGDIALOG_H
