#ifndef PRODUCTMODEL_H
#define PRODUCTMODEL_H

#include "iproductrepository.h"
#include "productentity.h"
#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>


namespace ProductInventory { class ProductRepository; }
namespace Business { class IInventoryRepository; }
namespace ProductInventory
{
class ProductModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ProductInventory::IProductRepository *repository READ repository WRITE setRepository NOTIFY repositoryChanged FINAL)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged FINAL)
    QML_ELEMENT
public:
    //using SalespersonId = int;
    using ProductId = int;

    enum class RoleNames
    {
        ProductIdRole = Qt::UserRole,
        ProductNameRole,
        PriceRole,
        BuyPriceRole,
        DescriptionRole,
        MaximumStockLevelRole,
        MinimumStockLevelRole,
        ReorderPointRole,
        QuantityAvailableRole,
        ImageUrlRole,
        SelectedQtyRole,
        CategoryIdRole,
        ImagesRole,
        SupplierIdRole,
        SupplierNameRole,
        SupplierPriceRole,
        BuyPriceHistoryRole,
        WarehouseNameRole,
    };

    enum class Status
    {
        Idle,
        Busy,
        Error
    };
    Q_ENUM(Status)

    explicit ProductModel(QObject *parent = nullptr);
    ProductModel(IProductRepository *newRepository, QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    int count() const;

    Q_INVOKABLE EntityLayer::Product get(int index) const;
    Q_INVOKABLE bool contains(const EntityLayer::Product &product) const;
    Q_INVOKABLE QModelIndex find(int productId) const;

    ProductInventory::IProductRepository *repository() const;
    void setRepository(ProductInventory::IProductRepository *newRepository);

    Status status() const;
    void setStatus(Status newStatus);

public slots:
    void update();
    void updateFromList(const QList<EntityLayer::Product> &products);

    void editFromList(const QVariantList &list) noexcept;

    void addProduct(const QVariantMap &map) noexcept;
    void append(const EntityLayer::Product &product);

    void clear();
    void remove(int index);
    void removeByProductId(ProductId id);

    void refreshBuyPriceHistory(ProductId id) noexcept;

signals:
    void repositoryChanged();
    void countChanged();
    void updated();
    void added();

    void productRepositoryChanged();

    void buyPriceHistoryRefreshed();

    void statusChanged();

private:
    void makeConnections() noexcept;
    QJsonObject findLatestPrice(const QJsonArray &array) const noexcept;

    /**
     * @brief editFromJson Edit the product in tho model that matches the id in json.
     * @param json A buy oreder details represented in json
     */
    void editFromJson(const QJsonObject &json) noexcept;

    QList<EntityLayer::Product> data_;
    IProductRepository *repository_;
    Status status_;
};
}

#endif // PRODUCTMODEL_H
