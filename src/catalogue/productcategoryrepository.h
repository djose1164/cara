#ifndef PRODUCTCATEGORYREPOSITORY_H
#define PRODUCTCATEGORYREPOSITORY_H

#include "iproductcategoryrepository.h"
#include <QObject>
#include <QNetworkAccessManager>


namespace Catalogue {
class ProductCategoryRepository : public QObject, public IProductCategoryRepository
{
    Q_OBJECT

public:
    ProductCategoryRepository(QObject *parent = nullptr);

public slots:
    void fetchAll() override;

signals:
    void allFetched(const QByteArray &data);

private:
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
};
}

#endif // PRODUCTCATEGORYREPOSITORY_H
