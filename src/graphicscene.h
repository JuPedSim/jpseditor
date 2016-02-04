#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include <QGraphicsScene>
#include <QPainter>

class GraphicScene : public QGraphicsScene
{
    Q_OBJECT

public:

    GraphicScene(QObject* parent=nullptr);
    GraphicScene(qreal x, qreal y, qreal w, qreal h)
        : QGraphicsScene(x, y, w, h)
    { }
    ~GraphicScene();
    void ChangeGridmode(const bool& stat);
    bool GetGridmode() const;
    void ChangeTranslation(qreal x, qreal y);
    void SetGrid(QString grid);
    void ChangeGridSize(const qreal& factor);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void DrawLineGrid(QPainter *painter, const QRectF &rect);
    void DrawPointGrid(QPainter *painter, const QRectF &rect);
private:
    bool _gridmode;
    qreal _translationX;
    qreal _translationY;
    QString _statgrid;
    qreal _gridSize;

};

#endif // GRAPHICSCENE_H
