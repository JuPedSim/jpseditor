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

protected slots:
    void sendState();

signals:
    void snapStart_endpoint(bool);
    void snapIntersections_point(bool);
    void snapCenter_point(bool);
    void snapSelectedLine_point(bool);

private:
    Ui::SnappingOptions *ui;
};

#endif // SNAPPINGOPTIONS_H
