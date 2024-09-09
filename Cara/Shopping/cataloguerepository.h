#ifndef CATALOGUEREPOSITORY_H
#define CATALOGUEREPOSITORY_H

#include "iproductcategoryrepository.h"
#include "iproductrepository.h"
#include "productentity.h"
#include <QObject>
#include <QQmlEngine>

namespace Product
{
class PictureRepository;
}
namespace Catalogue
{
class CatalogueRepository : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    using ProductId = int;
    using PictureId = int;
    using SupplierId = int;

    explicit CatalogueRepository(QObject *parent = nullptr);

public slots:
    void addProduct(const QVariantMap &product);

    void addPicture(ProductId productId, const QVariantList &pictureList);

    void deletePicture(PictureId pictureId);

    void readProductById(ProductId id);

    void fetchProducts();

    void fetchProductsFromSupplier(SupplierId supplierId);

    void editProduct(const QVariantMap &product);

    void deleteProduct(int productId);

    void addCategory(const QVariantMap &category);

    void fetchCategories();

    void editCategory(const QVariantMap &category);

    void deleteCategory(int categoryId);

signals:
    void categoriesFetched(const QByteArray &data);
    void productsFetched(const QByteArray &data);
    void productFetched(const QByteArray &data);
    void allProductsFetched(const QList<EntityLayer::Product> &products);

    void productAdded(const QByteArray &data);

    void productEdited();

    void errorOccurred(const QByteArray &data);

    void pictureAdded(const QByteArray &data);

    void pictureDeleted();

private:
    Product::PictureRepository *pictureRepository_;
    IProductRepository *productRepository_;
    IProductCategoryRepository *categoryRepository_;
};
} // namespace Catalogue

#endif // CATALOGUEREPOSITORY_H
