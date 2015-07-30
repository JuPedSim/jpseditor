#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include <QGraphicsScene>
#include <QPainter>

class GraphicScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphicScene();
    GraphicScene(QObject* parent=0);
    GraphicScene(qreal x, qreal y, qreal w, qreal h)
        : QGraphicsScene(x, y, w, h)
    { }
    ~GraphicScene();
    void ChangeGridmode(const bool& stat);
    bool GetGridmode() const;
    void ChangeTranslation(qreal x, qreal y);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
private:
    bool _gridmode;
    qreal _translationX;
    qreal _translationY;

};

#endif // GRAPHICSCENE_H
