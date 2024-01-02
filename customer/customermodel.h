#ifndef CUSTOMERMODEL_H
#define CUSTOMERMODEL_H

#include "customerrepository.h"
#include <QAbstractListModel>
#include <QQmlEngine>
#include <QJsonObject>

namespace Model {
class CustomerModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
    Q_PROPERTY(Business::CustomerRepository *repository READ repository WRITE setRepository NOTIFY repositoryChanged FINAL)
public:
    using SalespersonId = Business::CustomerRepository::SalespersonId;

    enum class RoleNames
    {
        CustomerIdRole = Qt::UserRole,
        AddressRole,
        SalespersonRole,
        ContactRole,
        FullnameRole
    };

    explicit CustomerModel(QObject *parent = nullptr);

    int count() const;

    void setRepository(Business::CustomerRepository * const newRepository);
    Business::CustomerRepository *repository() const;

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void countChanged();
    void repositoryChanged();
    void updated();

public slots:
    void insert(int index, const QJsonObject &data);
    void append(const QJsonObject &data);

    void update(SalespersonId salespersonId);

private:
    QList<QJsonObject> data_;
    Business::CustomerRepository *repository_;
};
}

#endif // CUSTOMERMODEL_H
