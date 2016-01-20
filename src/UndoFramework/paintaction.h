#ifndef PAINTACTION_H
#define PAINTACTION_H

#include "action.h"
#include <QLine>

class LineAction : public Action
{
public:
    LineAction();
    LineAction(const QString& name, const QString& type, const QLineF& oldLine);
    ~LineAction();

    QLineF GetOldLine() const;
private:
    QLineF _oldLine;
};

#endif // PAINTACTION_H
