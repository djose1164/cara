#ifndef COMMISSION_H
#define COMMISSION_H

#include "userentity.h"
#include "salesperson.h"
#include <QDateTime>

class QJsonObject;

namespace Billing
{

class CommissionItem
{
    Q_GADGET
    Q_PROPERTY(int productId READ productId WRITE setProductId NOTIFY productIdChanged FINAL)
    Q_PROPERTY(int quantity READ quantity WRITE setQuantity NOTIFY quantityChanged FINAL)
    Q_PROPERTY(int supplierId READ supplierId WRITE setSupplierId NOTIFY supplierIdChanged FINAL)
    Q_PROPERTY(double unitCommission READ unitCommission WRITE setUnitCommission NOTIFY unitCommissionChanged FINAL)
    Q_PROPERTY(QUrl imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged FINAL)
    Q_PROPERTY(CommissionId commissionId READ commissionId WRITE setCommissionId FINAL)
    Q_PROPERTY(QString productName READ productName WRITE setProductName FINAL)
    QML_VALUE_TYPE(commissionItem)
public:
    using CommissionId = int;

    CommissionItem();
    CommissionItem(int newProductId, int newQuantity, double newUnitCommission, int newSupplierId);

    QJsonObject toJson() const;

    static CommissionItem fromJson(const QJsonObject &json);
    static CommissionItem fromMap(const QVariantMap &map);

    int productId() const;
    void setProductId(int newProductId);

    int quantity() const;
    void setQuantity(int newQuantity);

    int supplierId() const;
    void setSupplierId(int newSupplierId);

    double unitCommission() const;
    void setUnitCommission(double newUnitCommission);

    QUrl imageUrl() const;
    void setImageUrl(const QUrl &newImageUrl);

    CommissionId commissionId() const;
    void setCommissionId(CommissionId newCommissionId);

    QUuid commissionUuid() const;
    void setCommissionUuid(const QUuid &newCommissionUuid);

    static void validateMap(const QVariantMap &map);

    QString productName() const;
    void setProductName(const QString &newProductName);

private:
    int productId_;
    int quantity_;
    int supplierId_;
    double unitCommission_;
    QUrl imageUrl_;
    CommissionId commissionId_;
    QUuid commissionUuid_;
    QString productName_;
};

class Commission
{
public:
    Commission();

    QJsonObject toJson() const;

    static Commission fromJson(const QJsonObject &json);

    static Commission fromMap(const QVariantMap &map);

    static void validateMap(const QVariantMap &map);

    double rate() const;
    void setRate(double newRate);

    double amount() const;
    void setAmount(double newAmount);

    int commissionId() const;
    void setCommissionId(int newCommissionId);

    int adminId() const;
    void setAdminId(int newAdminId);

    int salespersonId() const;
    void setSalespersonId(int newSalespersonId);

    QDateTime paymentDate() const;
    void setPaymentDate(const QDateTime &newPaymentDate);

    QDateTime createdAt() const;
    void setCreatedAt(const QDateTime &newCreatedAt);

    QList<CommissionItem> commissionItems() const;
    QList<CommissionItem> &commissionItems();
    void setCommissionItems(const QList<CommissionItem> &newCommissionItems);
    void setCommissionItems(QList<CommissionItem> &&newCommissionItems);

    Entity::Salesperson salesperson() const;
    void setSalesperson(const Entity::Salesperson &newSalesperson);

    Entity::UserEntity admin() const;
    void setAdmin(const Entity::UserEntity &newAdmin);

    bool isPatchable() const;

    /**
     * @brief isValid Check whether this commission has a uniqueCode, a valid amount
     * and its items are not empty.
     * @return true if the metions conditions are met, otherwise false.
     */
    bool isValid() const;

    double getGrossProfit() const;

    void calculateAmount();

    QUuid uniqueCode() const;
    void setUniqueCode(const QUuid &newUniqueCode);

private:
    void setCommissionItems(const QJsonArray &array);

    double rate_;
    double amount_;
    int commissionId_;
    int adminId_;
    int salespersonId_;
    QDateTime paymentDate_;
    QDateTime createdAt_;
    QList<CommissionItem> commissionItems_;
    Entity::Salesperson salesperson_;
    Entity::UserEntity admin_;
    QUuid uniqueCode_;
};

} // namespace Billing

QDebug operator<<(QDebug dbg, const Billing::Commission &c);

QDebug operator<<(QDebug dbg, const Billing::CommissionItem &ci);

#endif // COMMISSION_H
