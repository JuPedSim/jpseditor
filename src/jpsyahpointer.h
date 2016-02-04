#ifndef JPSYAHPOINTER_H
#define JPSYAHPOINTER_H
#include <QPointF>
#include <vector>
class jpsYAHPointer
{
public:
    jpsYAHPointer(const QPointF& point, const qreal& angle=0.0);
    jpsYAHPointer(const qreal& x, const qreal& y, const qreal& angle=0.0);
    ~jpsYAHPointer();

    const QPointF& GetPos() const;
    const qreal& GetDirection() const;

    void SetPos(const QPointF& point);
    void SetDirection(const qreal& angle);

    //Occurence
    const int& GetFirstFrame() const;
    const int& GetLastFrame() const;
    void SetFirstFrame(const int& frame);
    void SetLastFrame(const int& frame);

    //
    const QPointF& GetPosWhenFrame(const int& frame);
    const qreal& GetDirWhenFrame(const int& frame);

private:
    QPointF _cPos;
    qreal _cDirection;
    std::vector<QPointF> _allPos;
    std::vector<qreal> _allDir;

    //Occurence
    int _firstFrame;
    int _lastFrame;
};

#endif // JPSYAHPOINTER_H
