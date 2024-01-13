#ifndef PRODUCTREPOSITORY_H
#define PRODUCTREPOSITORY_H

#include "iproductrepository.h"

#include <QNetworkReply>
#include <QObject>
#include <QQmlEngine>
#include <QHttpPart>

namespace Catalogue {
class ProductRepository : public QObject, public IProductRepository
{
    Q_OBJECT

public:
    explicit ProductRepository(QObject *parent = nullptr);

public slots:
    void add(const QVariantMap &product) override;

    void update(const QVariantMap &product) override;

    void fetchAll() override;

signals:
    void added(const QByteArray &body);
    void updated();
    void allFetched(const QByteArray &data);
    void errorOccurred(const QByteArray &data);

private:
    void prepareImagePart(const QUrl imageUrl);

    void prepareJsonPart(const QVariantMap &product);

    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;

    QString base_;

    QHttpMultiPart *multiPart_;
};
}

#endif // PRODUCTREPOSITORY_H
