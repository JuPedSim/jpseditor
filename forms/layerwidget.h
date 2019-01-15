#ifndef LAYERWIDGET_H
#define LAYERWIDGET_H

#include <QWidget>

namespace Ui {
class Layerwidget;
}

class Layerwidget : public QWidget
{
    Q_OBJECT

public:
    explicit Layerwidget(QWidget *parent = nullptr);
    ~Layerwidget();

private:
    Ui::Layerwidget *ui;
};

#endif // LAYERWIDGET_H
