#ifndef RESTSALEREPOSITORY_H
#define RESTSALEREPOSITORY_H

#include <QQmlEngine>
#include "salerepository.h"

class QNetworkRequestFactory;
class QRestAccessManager;
class QRestReply;
namespace Billing
{

class RestSaleRepository : public SaleRepository
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit RestSaleRepository(QObject *parent = nullptr);

    // SaleRepository interface
    void fetchSales() override;

    void addSale(const Sale &sale) override;

    void setPagination(const Network::Pagination &newPagination) override;

    static QList<Sale> mapResponseToSales(const QJsonArray &arr);

private:
    void salesFetched(QRestReply &reply);

    std::shared_ptr<QNetworkRequestFactory> api_;
    QRestAccessManager *manager_;
    QString path_;
    Network::Pagination pagination_;
};

} // namespace Billing

#endif // RESTSALEREPOSITORY_H
