
#ifndef JPSEDITOR_TEMPLATE_H
#define JPSEDITOR_TEMPLATE_H

#include <QWidget>

namespace Ui {
    class TemplateWidget;
}

class TemplateWidget : public QWidget {
    Q_OBJECT

public:
    explicit TemplateWidget(QWidget *parent = nullptr);
    ~TemplateWidget();

protected slots:
    void pushButton_chooseClicked();
    void pushButton_cancelClicked();

signals:
    void sendTemplateName(QString name);

private:
    Ui::TemplateWidget *ui;
};


#endif //JPSEDITOR_TEMPLATE_H
