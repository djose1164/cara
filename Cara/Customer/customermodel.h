#ifndef CUSTOMERMODEL_H
#define CUSTOMERMODEL_H

#include "customerrepository.h"
#include <QAbstractListModel>
#include <QQmlEngine>
#include <QJsonObject>
#include <QJsonArray>

namespace Model {
class CustomerModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
public:
    using SalespersonId = Business::CustomerRepository::SalespersonId;

    enum class RoleNames
    {
        CustomerIdRole = Qt::UserRole,
        AddressRole,
        SalespersonRole,
        PersonRole,
        FullnameRole,
        ForenameRole,
        SurnameRole
    };

    explicit CustomerModel(QObject *parent = nullptr);

    int count() const;

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
    void clear();

    /**
     * @brief set Clear the model & set the data.
     * @param data The new data to be set.
     */
    void set(const QJsonArray &data);

private:
    QList<QJsonObject> data_;
};
}

#endif // CUSTOMERMODEL_H
