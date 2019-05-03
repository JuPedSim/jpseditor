//
// Created by Tao Zhong on 2019-03-27.
//

#ifndef JPSEDITOR_JPSGOALLISTMODEL_H
#define JPSEDITOR_JPSGOALLISTMODEL_H

#include <QAbstractListModel>
#include "src/models/jpselementlistmodel.h"
#include "src/jpsgoal.h"

class JPSGoalListModel : public  JPSElementListModel
{
    Q_OBJECT

public:
    explicit  JPSGoalListModel(QObject *parent = nullptr);
    explicit  JPSGoalListModel(QList<JPSGoal *> &goals, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QList<JPSGoal *> goalsList();
    void setGoalsList(QList<JPSGoal *> goals);

private:
    QList<JPSGoal *> lst;
};


#endif //JPSEDITOR_JPSGOALLISTMODEL_H
