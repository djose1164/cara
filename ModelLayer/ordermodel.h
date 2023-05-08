#ifndef ORDERMODEL_H
#define ORDERMODEL_H

#include "detailsentity.h"
#include <QAbstractListModel>
#include <QtQml>

namespace ModelLayer {
using EntityLayer::DetailsEntity;
class OrderModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum class RoleNames
    {
        ProductRole = Qt::UserRole,
        QuantityRole,
        TotalRole,
    };

    explicit OrderModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    //bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    //Qt::ItemFlags flags(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;

    int count() const;
    Q_INVOKABLE DetailsEntity *get(int index);

signals:
    void countChanged(int count);

public slots:
    void insert(int index, DetailsEntity *entityentity);
    void append(const DetailsEntity *entity);
    void remove(int index);
    void clear();

private:
    QList<EntityLayer::DetailsEntity *> data_;
    QHash<int, QByteArray> roleNames_;
};
}

#endif // ORDERMODEL_H
