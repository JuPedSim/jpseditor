#ifndef ACTION_H
#define ACTION_H
#include <QString>

class Action
{
public:
    Action();
    Action(const QString& name, const QString& type);
    ~Action();
    //Getter
    const QString& GetName() const;
    const QString& GetType() const;


private:
    QString _name;
    QString _type;


};

#endif // ACTION_H
