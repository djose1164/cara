#ifndef SQLITECOMMISSIONMODEL_H
#define SQLITECOMMISSIONMODEL_H

#include "ProductInventory/sqliteproductmodel.h"
#include "abstractcommissionmodel.h"
#include <QQmlEngine>
#include <QSqlQueryModel>

namespace Billing
{

class CommissionItem;
class Commission;

class SQLiteCommissionModel : public QSqlQueryModel, public AbstractCommissionModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(ProductInventory::SQLiteProductModel *productModel
               READ productModel
               WRITE setProductModel
               NOTIFY productModelChanged FINAL)
    Q_INTERFACES(QQmlParserStatus)
    QML_ELEMENT
public:
    // The select query this follow this column name order
    enum class RoleNames
    {
        CommissionIdRole = Qt::UserRole + 1,
        UniqueCodeRole,
        AdminIdRole,
        SellerIdRole,
        RateRole,
        AmountRole,
        PaymentDateRole,
        CreatedAtRole,
        SellerNameRole,
        AdminNameRole,
        CommissionItemsRole,
        GrossProfitRole
    };

    explicit SQLiteCommissionModel(QObject *parent = nullptr);


    // QAbstractItemModel interface
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    ProductInventory::SQLiteProductModel *productModel() const;
    void setProductModel(ProductInventory::SQLiteProductModel *newProductModel);

    QAbstractItemModel *itemModel() override;
    void scheduleDelete() override;

    void fetchCommissionsBySellerId(SellerId sellerId) override;

    // QQmlParserStatus interface
    void classBegin() override;
    void componentComplete() override;

public slots:
    void append(const QVariantMap &map) override;

signals:
    void productModelChanged();
    void commissionAddedSuccessfully();
    void errorOccurred(Error error, const QString &errorString);

private:
    void generateRoleNames();
    void insertCommission(const Commission &commission);
    bool insertCommissionItem(const CommissionItem &ci);
    void fetchCommissions() override;
    QList<CommissionItem> fetchCommissionItems(const QUuid &commissionId) const;

    // Calculate the commission unit of each CommissionItem and set it.
    void updateCommissionUnit(QList<CommissionItem> &ciList);

    ProductInventory::SQLiteProductModel *productModel_;
};

} // namespace Billing


#endif // SQLITECOMMISSIONMODEL_H
