//
// Created by Tao Zhong on 2019-03-21.
//

#ifndef JPSEDITOR_JPSELEMENTLISTMODEL_H
#define JPSEDITOR_JPSELEMENTLISTMODEL_H

#include "src/jpssource.h"
#include <QAbstractListModel>
#include <QGraphicsItem>

class JPSElementListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit JPSElementListModel(QObject *parent = nullptr);
    explicit JPSElementListModel(QList<QGraphicsItem *> &items, QObject *parent = nullptr);


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
//    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    bool clearItemData(const QModelIndex &index) override;
#endif

//    Qt::ItemFlags flags(const QModelIndex &index) const override;

//    QList<QGraphicsItem* > getElementsList() const;
//    void setElementsList(const QList<QGraphicsItem* > &elements);

    virtual ~JPSElementListModel();

private:
    QList<QGraphicsItem *> lst;
};


#endif //JPSEDITOR_JPSELEMENTLISTMODEL_H
