#ifndef ATTRIBUTETABWIDGET_H
#define ATTRIBUTETABWIDGET_H

#include <QTabWidget>
#include "forms/addlayerdialog.h"

namespace Ui {
class attributeTabWidget;
}

class attributeTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit attributeTabWidget(QWidget *parent = nullptr);
    ~attributeTabWidget();

private slots:
    void on_addButton_clicked();

private:
    Ui::attributeTabWidget *ui;
};

#endif // ATTRIBUTETABWIDGET_H
