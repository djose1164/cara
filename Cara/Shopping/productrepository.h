#ifndef PRODUCTREPOSITORY_H
#define PRODUCTREPOSITORY_H

#include "iproductrepository.h"
#include "productentity.h"

#include <QNetworkReply>
#include <QObject>
#include <QQmlEngine>
#include <QHttpPart>
#include <QNetworkRequestFactory>

namespace Catalogue {
class ProductRepository : public QObject, public IProductRepository
{
    Q_OBJECT

public:
    explicit ProductRepository(QObject *parent = nullptr);

    QString path() const;
    void setPath(const QString &newPath);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

public slots:
    void add(const QVariantMap &product) override;

    QNetworkReply *read(ProductId id) override;

    void update(const QVariantMap &product) override;

    void fetchAll() override;

    QNetworkReply *addPicture(ProductId id, const QVariantList &pictureList) override;

    QNetworkReply *deletePicture(PictureId pictureId) override;

signals:
    void added(const QByteArray &body);
    void updated();
    void allFetched(const QByteArray &data);
    void errorOccurred(const QByteArray &data);
    void allProductsFetched(const QList<EntityLayer::Product> &products);

private:
    void prepareImagePart(const QVariantList &pictureList);

    void prepareJsonPart(const QVariantMap &product);

    QNetworkRequest requestWithBoundary(QAnyStringView endpoint = {}) const;

    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;

    QHttpMultiPart *multiPart_;

    QString path_;
    QString errorString_;
    std::shared_ptr<QNetworkRequestFactory> api_;
};
}

#endif // PRODUCTREPOSITORY_H
