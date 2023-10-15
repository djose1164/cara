#ifndef CATALOGUEREPOSITORY_H
#define CATALOGUEREPOSITORY_H

#include "iproductcategoryrepository.h"
#include "iproductrepository.h"
#include <QObject>
#include <QQmlEngine>

namespace Catalogue {
class CatalogueRepository : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit CatalogueRepository(QObject *parent = nullptr);

public slots:
    void addProduct(const QVariantMap &product);

    void fetchProducts();

    void editProduct(const QVariantMap &product);

    void deleteProduct(int productId);

    void addCategory(const QVariantMap &category);

    void fetchCategories();

    void editCategory(const QVariantMap &category);

    void deleteCategory(int categoryId);

signals:
    void categoriesFetched(const QByteArray &data);
    void productsFetched(const QByteArray &data);

    void productAdded(const QByteArray &data);

    void productEdited();

    void errorOccurred(const QByteArray &data);

private:
    IProductRepository *productRepository_;
    IProductCategoryRepository *categoryRepository_;
};
}

#endif // CATALOGUEREPOSITORY_H
