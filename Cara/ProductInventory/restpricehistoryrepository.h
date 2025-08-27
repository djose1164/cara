#ifndef RESTPRICEHISTORYREPOSITORY_H
#define RESTPRICEHISTORYREPOSITORY_H

#include <QNetworkRequestFactory>
#include <QProperty>
#include <QQmlEngine>
#include <QRestAccessManager>
#include "pricehistoryrepository.h"

namespace ProductInventory
{

class RestPriceHistoryRepository : public PriceHistoryRepository
{
    Q_OBJECT
    Q_PROPERTY(QString priceType MEMBER priceType_ BINDABLE bindablePriceType FINAL)
    QML_ELEMENT
public:
    explicit RestPriceHistoryRepository(QObject *parent = nullptr);

    QBindable<QString> bindablePriceType();

    // PriceHistoryRepository interface
public slots:
    void addNewPrice(const QVariantMap &newPrice) noexcept override;

    void fetchPriceHistoryBy(ProductId productId) noexcept override;

private:
    void newPriceAdded(QRestReply &reply);

    void priceHistoryFetched(QRestReply &reply) noexcept;

private:
    static void validateMap(const QVariantMap &map);

    QRestAccessManager *manager_;
    std::shared_ptr<QNetworkRequestFactory> api_;
    QProperty<QString> priceType_;
};

} // namespace ProductInventory

#endif // RESTPRICEHISTORYREPOSITORY_H
