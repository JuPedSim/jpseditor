#ifndef ATTRIBUTETABWIDGET_H
#define ATTRIBUTETABWIDGET_H

#include <QTabWidget>

namespace Ui {
class attributeTabWidget;
}

class attributeTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit attributeTabWidget(QWidget *parent = nullptr);
    ~attributeTabWidget();

private:
    Ui::attributeTabWidget *ui;
};

#endif // ATTRIBUTETABWIDGET_H
