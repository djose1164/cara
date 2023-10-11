#include "cataloguerepository.h"
#include "productrepository.h"
#include "productcategoryrepository.h"

namespace Catalogue {
CatalogueRepository::CatalogueRepository(QObject *parent)
    : QObject{parent}
    , productRepository_{new ProductRepository(this)}
    , categoryRepository_{new ProductCategoryRepository(this)}
{

}

void CatalogueRepository::addProduct(const QVariantMap &product)
{
    productRepository_->add(product);

    const auto productRepository{dynamic_cast<ProductRepository *>(productRepository_)};
    connect(productRepository, &ProductRepository::added, this, [&](const QByteArray &data) {
        emit productAdded(data);
    });
}

void CatalogueRepository::fetchProducts()
{
    productRepository_->fetchAll();
    const auto productRepository{dynamic_cast<ProductRepository *>(productRepository_)};

    connect(productRepository, &ProductRepository::allFetched, this, [&](const QByteArray &data) {
        emit productsFetched(data);
    });
}

void CatalogueRepository::deleteProduct(int productId)
{

}

void CatalogueRepository::editCategory(const QVariantMap &category)
{

}

void CatalogueRepository::deleteCategory(int categoryId)
{

}

void CatalogueRepository::addCategory(const QVariantMap &category)
{

}

void CatalogueRepository::fetchCategories()
{
    categoryRepository_->fetchAll();

    const auto categoryRepository{dynamic_cast<ProductCategoryRepository *>(categoryRepository_)};
    connect(categoryRepository, &ProductCategoryRepository::allFetched, this, [&](const QByteArray &data) {
        emit categoriesFetched(data);
    });
}

void CatalogueRepository::editProduct(const QVariantMap &product)
{
    productRepository_->update(product);

    const auto repository{dynamic_cast<ProductRepository *>(productRepository_)};
    connect(repository, &ProductRepository::updated, this, [&] {
        emit productEdited();
    });
}
}
