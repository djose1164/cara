#ifndef SALESPERSON_H
#define SALESPERSON_H

#include "ipersonname.h"

#include <QObject>
#include <QQmlEngine>
#include <QJsonObject>

namespace Entity {
class Salesperson : public IPersonName
{
    Q_GADGET
    Q_PROPERTY(int salespersonId READ salespersonId WRITE setSalespersonId FINAL)
    Q_PROPERTY(SalespersonTypes salespersonType READ salespersonType WRITE setSalespersonType FINAL)
    Q_PROPERTY(int creditLimit READ creditLimit WRITE setCreditLimit FINAL)
    Q_PROPERTY(int creditConsumed READ creditConsumed WRITE setCreditConsumed  FINAL)
    Q_PROPERTY(int creditAvailable READ creditAvailable WRITE setCreditAvailable FINAL)
    Q_PROPERTY(int adminId READ adminId WRITE setAdminId FINAL)
    Q_PROPERTY(int userId READ userId WRITE setUserId NOTIFY userIdChanged FINAL)
    Q_PROPERTY(QString warehouseAddress READ warehouseAddress WRITE setWarehouseAddress FINAL)
    Q_PROPERTY(QString salespersonTypeName READ salespersonTypeName WRITE setSalespersonTypeName FINAL)
    Q_PROPERTY(OrganizationId organizationId READ organizationId WRITE setOrganizationId FINAL)
    Q_PROPERTY(QString forename READ forename WRITE setForename FINAL)
    Q_PROPERTY(QString surname READ surname WRITE setSurname FINAL)
    QML_VALUE_TYPE(salesperson)
    QML_STRUCTURED_VALUE
public:
    using SalespersonId = int;
    using UserId = int;
    using OrganizationId = int;

    enum class SalespersonTypes
    {
        Administrator = 1,
        Associate
    };
    Q_ENUM(SalespersonTypes);

    static Salesperson fromMap(const QVariantMap &data);
    static Salesperson fromJson(const QByteArray &data);
    static Salesperson fromJsonObject(const QJsonObject &jsonObject);

    static const int userTypeId;

    Salesperson();

    bool operator==(const Salesperson &other) const;
    bool operator!=(const Salesperson &other) const;

    QString username() const;
    void setUsername(const QString &newUsername);

    QString password() const;
    void setPassword(const QString &newPassword);

    QString email() const;
    void setEmail(const QString &newEmail);

    QString forename() const override;
    void setForename(const QString &newForename);

    QString surname() const override;
    void setSurname(const QString &newSurname);

    int creditLimit() const;
    void setCreditLimit(int newCreditLimit);

    bool isPosteable() const;

    QByteArray toJson() const;

    int creditConsumed() const;
    void setCreditConsumed(int newCreditConsumed);

    int creditAvailable() const;
    void setCreditAvailable(int newCreditAvailable);

    SalespersonTypes salespersonType() const;
    void setSalespersonType(const SalespersonTypes &newSalespersonType);

    int salespersonId() const;
    void setSalespersonId(SalespersonId newSalespersonId);

    UserId adminId() const;
    void setAdminId(UserId newAdminId);

    UserId userId() const;
    void setUserId(UserId newUserId);

    bool isValid() const;

    bool isAssociate() const;
    bool isAdministrator() const;

    QString warehouseAddress() const;
    void setWarehouseAddress(const QString &newWarehouseAddress);

    QString salespersonTypeName() const;
    void setSalespersonTypeName(const QString &newSalespersonTypeName);

    OrganizationId organizationId() const;
    void setOrganizationId(OrganizationId newOrganizationId);

    QString fullname() const override;

private:
    QJsonObject prepareUser() const;

    QJsonObject prepareWarehouse() const;

    QJsonObject prepareContact() const;

private:
    SalespersonId salespersonId_;
    UserId adminId_;
    UserId userId_;
    OrganizationId organizationId_;

    QString username_;
    QString password_;
    QString email_;

    QString forename_;
    QString surname_;
    QString warehouseAddress_;
    QString salespersonTypeName_;

    int creditLimit_;
    int creditConsumed_;
    int creditAvailable_;

    SalespersonTypes salespersonType_;
};
}

namespace SalespersonForeign
{
Q_NAMESPACE
QML_FOREIGN_NAMESPACE(Entity::Salesperson)
QML_NAMED_ELEMENT(Salesperson)
}

#endif // SALESPERSON_H
