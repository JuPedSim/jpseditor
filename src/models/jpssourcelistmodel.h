//
// Created by Tao Zhong on 2019-03-21.
//

#ifndef JPSEDITOR_JPSSOURCELISTMODEL_H
#define JPSEDITOR_JPSSOURCELISTMODEL_H

#include <QAbstractListModel>
#include "src/models/jpselementlistmodel.h"
#include "src/jpssource.h"

class JPSSourceListModel : public JPSElementListModel
{
    Q_OBJECT

public:
    explicit JPSSourceListModel(QObject *parent = nullptr);
    explicit JPSSourceListModel(QList<JPSSource *> &sources, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QList<JPSSource *> sourcesList();
    void setSourceList(QList<JPSSource *> sources);

private:
    QList<JPSSource *> lst;
};


#endif //JPSEDITOR_JPSSOURCELISTMODEL_H
