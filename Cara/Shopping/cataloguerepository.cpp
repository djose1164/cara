#include "cataloguerepository.h"
#include <QJsonDocument>
#include "productcategoryrepository.h"
#include "productpicturerepository.h"
#include "productrepository.h"

using namespace Qt::StringLiterals;

namespace
{
constexpr auto productsField{"products"_L1};
}

namespace Catalogue {
CatalogueRepository::CatalogueRepository(QObject *parent)
    : QObject{parent}
    , pictureRepository_{new ProductInventory::ProductPictureRepository{this}}
    , productRepository_{new ProductRepository(pictureRepository_, this)}
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
    if (productId < 1)
    {
        const QByteArray errorStr{"CatalogueRepository::addPicture: productId is invalid"_ba};
        qWarning() << errorStr;
        emit errorOccurred(errorStr);
        return;
    }
    else if (pictureList.isEmpty())
    {
        const QByteArray errorStr{"CatalogueRepository::addPicture: pictureList is empty"_ba};
        qWarning() << errorStr;
        emit errorOccurred(errorStr);
        return;
    }

    const auto reply{pictureRepository_->addPicture(productId, pictureList)};
    if (!reply)
        return;

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

void CatalogueRepository::deletePicture(PictureId pictureId)
{
    if (pictureId < 1)
    {
        emit errorOccurred("Invalid pictureId: %1"_L1
                               .arg(QString::number(pictureId))
                               .toUtf8());
        return;
    }

    const auto reply{pictureRepository_->deletePicture(pictureId)};

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

    connect(productRepository, &ProductRepository::allFetched,
            this, &CatalogueRepository::productsFetched);

    connect(productRepository, &ProductRepository::allProductsFetched,
            this, &CatalogueRepository::allProductsFetched);
}

void CatalogueRepository::fetchProductsFromSupplier(SupplierId supplierId)
{
    if (supplierId < 1)
    {
        const auto errorStr{"Invalid supplierId"_ba};
        qWarning() << "CatalogueRepository::fetchProductsFromSupplier: " << errorStr;
        emit errorOccurred(errorStr);
        return;
    }

    const auto reply{productRepository_->fetchFromSupplier(supplierId)};
    connect(reply,
            &QNetworkReply::finished,
            this,
            [reply, this]
            {
                QRestReply restReply{reply};
                if (!restReply.isSuccess())
                {
                    emit errorOccurred(restReply.readBody());
                    return;
                }

                if (const auto json{restReply.readJson()};
                    json && json->isObject() && json->object().contains("products"_L1))
                {
                    emit productsFetched(json->toJson());
                }
                else
                {
                    emit errorOccurred("products is not an array"_ba);
                }
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
