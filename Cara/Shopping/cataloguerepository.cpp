#include "cataloguerepository.h"
#include "productrepository.h"
#include "productcategoryrepository.h"
#include <QJsonDocument>

namespace Catalogue {
CatalogueRepository::CatalogueRepository(QObject *parent)
    : QObject{parent}
    , productRepository_{new ProductRepository(this)}
    , categoryRepository_{new ProductCategoryRepository(this)}
{
    const auto repository{dynamic_cast<ProductRepository *>(productRepository_)};
    connect(repository, &ProductRepository::errorOccurred, this, &CatalogueRepository::errorOccurred);
}

void CatalogueRepository::addProduct(const QVariantMap &product)
{
    productRepository_->add(product);

    const auto productRepository{dynamic_cast<ProductRepository *>(productRepository_)};
    connect(productRepository, &ProductRepository::added, this, [&](const QByteArray &data) {
        emit productAdded(data);
    });
}

void CatalogueRepository::addPicture(ProductId productId, const QVariantList &pictureList)
{
    const auto reply{productRepository_->addPicture(productId, pictureList)};

    connect(reply, &QNetworkReply::finished, this, [reply, this] {
        const QByteArray data{reply->readAll()};
        if (reply->error() != QNetworkReply::NoError)
        {
            emit errorOccurred(data);
            return;
        }

        emit pictureAdded(data);

        reply->deleteLater();
    });
}

void CatalogueRepository::deletePicture(IProductRepository::PictureId pictureId)
{
    const auto reply{productRepository_->deletePicture(pictureId)};

    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        const QByteArray data{reply->readAll()};
        if (reply->error() != QNetworkReply::NoError)
        {
            emit errorOccurred(data);
            return;
        }

        emit pictureDeleted();

        reply->deleteLater();
    });
}

void CatalogueRepository::readProductById(ProductId id)
{
    const auto reply{productRepository_->read(id)};
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        const auto doc{QJsonDocument::fromJson(reply->readAll())};
        if (reply->error() != QNetworkReply::NoError)
        {
            emit errorOccurred(doc.toJson());
            return;
        }

        emit productFetched(doc.toJson());

        reply->deleteLater();
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
