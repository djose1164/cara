#ifndef PRODUCTREPOSITORY_H
#define PRODUCTREPOSITORY_H

#include "ishoppingproductrepository.h"
#include "productentity.h"

#include <QNetworkReply>
#include <QObject>
#include <QQmlEngine>
#include <QHttpPart>
#include <QNetworkRequestFactory>

namespace Product
{
class PictureRepository;
}

namespace Catalogue
{
class ProductRepository : public QObject, public IShoppingProductRepository
{
    Q_OBJECT

public:
    using PictureId = int;

    explicit ProductRepository(QObject *parent = nullptr);
    ProductRepository(Product::PictureRepository *pictureRepository, QObject *parent = nullptr);

    QString path() const;
    void setPath(const QString &newPath);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

    static void validateMap(const QVariantMap &map);

public slots:
    void add(const QVariantMap &product) override;

    QNetworkReply *read(ProductId id) override;

    void update(const QVariantMap &product) override;

    void fetchAll() override;

    QNetworkReply *addPicture(ProductId id, const QVariantList &pictureList);
    QNetworkReply *deletePicture(PictureId pictureId);

    // IProductRepository interface
    QNetworkReply *fetchFromSupplier(SupplierId supplierId) override;

signals:
    void added(const QByteArray &body);
    void updated();
    void allFetched(const QByteArray &data);
    void errorOccurred(const QByteArray &data);
    void allProductsFetched(const QList<EntityLayer::Product> &products);

private:
    void prepareJsonPart(const QVariantMap &product);

    QNetworkRequest requestWithBoundary(QAnyStringView endpoint = {}) const;

    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;

    QHttpMultiPart *multiPart_;

    QString path_;
    QString errorString_;
    std::shared_ptr<QNetworkRequestFactory> api_;
    Product::PictureRepository *pictureRepository_;
};
} // namespace Catalogue

#endif // PRODUCTREPOSITORY_H
