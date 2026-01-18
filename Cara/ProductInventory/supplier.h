#ifndef SUPPLIER_H
#define SUPPLIER_H

#include <QQmlEngine>
#include <QUuid>

namespace ProductInventory
{

class Supplier
{
    Q_GADGET
public:
    using SupplierId = int;

    Supplier();

    QString name() const;
    void setName(const QString &newName);

    QUuid uniqueCode() const;
    void setUniqueCode(const QUuid &newUniqueCode);

    SupplierId supplierId() const;
    void setSupplierId(SupplierId newSupplierId);

    bool isValid() const;

    static Supplier fromJson(const QJsonObject &json);

private:
    SupplierId supplierId_;
    QString name_;
    QUuid uniqueCode_;
};

} // namespace ProductInventory

#endif // SUPPLIER_H
