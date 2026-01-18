#ifndef DATAPRELOADER_H
#define DATAPRELOADER_H

#include "supplier.h"
#include <QObject>
#include <QQmlEngine>

namespace EntityLayer { class Product; }

namespace Entity { class Salesperson; }

namespace ProductInventory
{
class Supplier;
class AbstractSupplierRepository;
class IProductRepository;
}

namespace Seller { class AbstractSellerRepository; }

namespace Billing
{

class DataPreloader : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(int adminId READ adminId WRITE setAdminId NOTIFY adminIdChanged FINAL)
    Q_INTERFACES(QQmlParserStatus)
    QML_ELEMENT
public:
    using AdminId = int;
    explicit DataPreloader(QObject *parent = nullptr);

    AdminId adminId() const;
    void setAdminId(AdminId newAdminId);

    // QQmlParserStatus interface
    void classBegin() override;
    void componentComplete() override;

public slots:
    void preload();

signals:
    void errorOccurred(const QString &errorString);

    void adminIdChanged();

private slots:
    void suppliersFetched(const QList<ProductInventory::Supplier> &supplierList);
    void sellersFetched(const QList<Entity::Salesperson> &sellerList);
    void productsFetched(const QList<EntityLayer::Product> &productList);

private:
    static bool insertSeller(const Entity::Salesperson &seller);

    ProductInventory::AbstractSupplierRepository *supplierRepo_;
    ProductInventory::IProductRepository *productRepo_;
    Seller::AbstractSellerRepository *sellerRepo_;

    AdminId adminId_;
};

} // namespace Billing

#endif // DATAPRELOADER_H
