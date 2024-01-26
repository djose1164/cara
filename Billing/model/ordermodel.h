#ifndef ORDERMODEL_H
#define ORDERMODEL_H

#include "detailsentity.h"
#include <QAbstractListModel>
#include <QtQml>

namespace Model
{
using EntityLayer::DetailsEntity, EntityLayer::JsonExportable;
class OrderModel : public QAbstractListModel, public JsonExportable
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
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QHash<int, QByteArray> roleNames() const override;

    int count() const;
    Q_INVOKABLE DetailsEntity *get(int index);
    Q_INVOKABLE QByteArray toJson() const override;

signals:
    void countChanged(int count);

public slots:
    void insert(int index, DetailsEntity *entityentity);
    void append(DetailsEntity *const entity);
    void remove(int index);
    void clear();

private:
    QList<DetailsEntity *> data_;
    QHash<int, QByteArray> roleNames_;
};
}

#endif // ORDERMODEL_H
