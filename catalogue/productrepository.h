#ifndef PRODUCTREPOSITORY_H
#define PRODUCTREPOSITORY_H

#include "iproductrepository.h"

#include <QNetworkReply>
#include <QObject>
#include <QQmlEngine>

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
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;

    const QString base_;
};
}

#endif // PRODUCTREPOSITORY_H
