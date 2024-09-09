#ifndef SALEREPOSITORY_H
#define SALEREPOSITORY_H

#include <QObject>
#include <QQmlEngine>
#include "Network/pagination.h"

namespace Billing
{

class Sale;

class SaleRepository : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("This is an abstract class.")
public:
    using QObject::QObject;

    virtual ~SaleRepository() = default;

    virtual void fetchSales() = 0;

    virtual void addSale(const Sale &sale) = 0;

    virtual void setPagination(const Network::Pagination &newPagination) = 0;

signals:
    void fetched(const QList<Sale> &sales, const Network::Pagination &pagination = {});
    void added(const QJsonObject &json);
    void errorOccurred(const QByteArray &error = {});
};

} // namespace Billing

#endif // SALEREPOSITORY_H
