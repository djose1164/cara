#ifndef SALESPERSON_H
#define SALESPERSON_H

#include <QObject>
#include <QQmlEngine>

namespace Entity {
class Salesperson
{
    Q_GADGET
    QML_VALUE_TYPE(salesperson)

    Q_PROPERTY(QString username READ username WRITE setUsername FINAL)
    Q_PROPERTY(QString password READ password WRITE setPassword FINAL)
    Q_PROPERTY(QString email READ email WRITE setEmail FINAL)
    Q_PROPERTY(QString forename READ forename WRITE setForename FINAL)
    Q_PROPERTY(QString surname READ surname WRITE setSurname FINAL)
    Q_PROPERTY(int creditLimit READ creditLimit WRITE setCreditLimit FINAL)
    Q_PROPERTY(int creditConsumed READ creditConsumed WRITE setCreditConsumed  FINAL)
    Q_PROPERTY(int creditAvailable READ creditAvailable WRITE setCreditAvailable FINAL)
    Q_PROPERTY(SalespersonTypes salespersonType READ salespersonType WRITE setSalespersonType FINAL)
    Q_PROPERTY(int salespersonId READ salespersonId WRITE setSalespersonId FINAL)
public:
    enum class SalespersonTypes
    {
        Administrator = 1,
        Associate
    };
    Q_ENUM(SalespersonTypes);

    static Salesperson fromMap(const QVariantMap &data);
    static Salesperson fromJson(const QByteArray &data);

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

    QString forename() const;
    void setForename(const QString &newForename);

    QString surname() const;
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
    void setSalespersonId(int newSalespersonId);

private:
    QJsonObject prepareUser() const;

    QJsonObject prepareWarehouse() const;

    QJsonObject prepareContact() const;

private:
    int salespersonId_;
    QString username_;
    QString password_;
    QString email_;

    QString forename_;
    QString surname_;

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
