#ifndef SNAPPINGOPTIONS_H
#define SNAPPINGOPTIONS_H

#include <QWidget>

namespace Ui {
class SnappingOptions;
}

class SnappingOptions : public QWidget
{
    Q_OBJECT

public:
    explicit SnappingOptions(QWidget *parent = nullptr);
    ~SnappingOptions();

private:
    Ui::SnappingOptions *ui;
};

#endif // SNAPPINGOPTIONS_H
