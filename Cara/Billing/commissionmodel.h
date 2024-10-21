#ifndef COMMISSIONMODEL_H
#define COMMISSIONMODEL_H

#include "commission.h"
#include "commissionrepository.h"
#include <QAbstractListModel>
#include <QProperty>
#include <QQmlEngine>

namespace Billing
{

class CommissionModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Billing::CommissionRepository *repository READ repository WRITE setRepository NOTIFY repositoryChanged FINAL)
    Q_PROPERTY(bool busy READ busy BINDABLE bindableBusy FINAL)
    QML_ELEMENT
public:
    enum class RoleNames
    {
        CommissionIdRole = Qt::UserRole,
        AdminIdRole,
        SalespersonIdRole,
        RateRole,
        AmountRole,
        PaymentDateRole,
        CreatedAtRole,
        SellerIdRole,
        SellerNameRole,
        AdminNameRole,
        CommissionItemsRole,
        GrossProfitRole
    };

    Q_ENUM(Billing::CommissionRepository::Error)

    explicit CommissionModel(QObject *parent = nullptr);

    CommissionRepository *repository() const;
    void setRepository(CommissionRepository *newRepository);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    bool busy() const;
    QBindable<bool> bindableBusy();

public slots:
    void append(const QVariantMap &map);
    void fetchCommissions();

private slots:
    void commissionsFetched(const QList<Commission> &commissionList);

    void commissionAdded(const Commission &commission);

    void commissionPatched(const QList<int> &roles);

signals:
    void errorOccurred(const QString &errorString, CommissionRepository::Error error);

    void repositoryChanged();

    void commissionAddedSuccessfully();

private:
    void setBusy(bool newBusy);

    void patch(RoleNames role);

    QList<Commission> data_;
    CommissionRepository *repository_;
    QProperty<bool> busy_;
};

} // namespace Billing

#endif // COMMISSIONMODEL_H
