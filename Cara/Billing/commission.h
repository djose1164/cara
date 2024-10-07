#ifndef COMMISSION_H
#define COMMISSION_H

#include "Salesperson/salesperson.h"
#include "User/userentity.h"
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
    QML_VALUE_TYPE(commissionItem)
public:
    CommissionItem();
    CommissionItem(int newProductId, int newQuantity, double newUnitCommission, int newSupplierId);

    QJsonObject toJson() const;

    static CommissionItem fromJson(const QJsonObject &json);

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

private:
    int productId_;
    int quantity_;
    int supplierId_;
    double unitCommission_;
    QUrl imageUrl_;
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
    void setCommissionItems(const QList<CommissionItem> &newCommissionItems);
    void setCommissionItems(QList<CommissionItem> &&newCommissionItems);

    Entity::Salesperson salesperson() const;
    void setSalesperson(const Entity::Salesperson &newSalesperson);

    Entity::UserEntity admin() const;
    void setAdmin(const Entity::UserEntity &newAdmin);

    bool isPatchable() const;

    double calculateGrossProfit() const;

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
};

} // namespace Billing

#endif // COMMISSION_H
