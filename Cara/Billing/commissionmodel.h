#ifndef COMMISSIONMODEL_H
#define COMMISSIONMODEL_H

#include "abstractcommissionmodel.h"
#include "commission.h"
#include "commissionrepository.h"
#include <QAbstractListModel>
#include <QProperty>
#include <QQmlEngine>

namespace Billing
{

class CommissionModel : public QAbstractListModel, public AbstractCommissionModel
{
    Q_OBJECT
    Q_PROPERTY(Billing::CommissionRepository *repository READ repository WRITE setRepository NOTIFY repositoryChanged FINAL)
    Q_PROPERTY(bool busy READ busy BINDABLE bindableBusy FINAL)
    QML_ELEMENT
public:
    using SellerId = int;

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

    QAbstractItemModel *itemModel() override;
    void scheduleDelete() override;

    void fetchCommissionsBySellerId(SellerId sellerId) override;

public slots:
    void append(const QVariantMap &map) override;
    void fetchCommissions() override;

private slots:
    void commissionsFetched(const QList<Billing::Commission> &commissionList);

    void commissionAdded(const Billing::Commission &commission);

    void commissionPatched(const QList<int> &roles);

signals:
    void repositoryChanged();

    void commissionAddedSuccessfully();
    void modelUpdated();

    void errorOccurred(CommissionRepository::Error error, const QString &errorString);

private:
    void setBusy(bool newBusy);

    void patch(RoleNames role);

    QList<Commission> data_;
    QPointer<CommissionRepository> repository_;
    QProperty<bool> busy_;
};

} // namespace Billing

#endif // COMMISSIONMODEL_H
