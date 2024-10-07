#ifndef PRICEHISTORYREPOSITORY_H
#define PRICEHISTORYREPOSITORY_H

#include <QQmlEngine>

namespace Product
{

class PriceHistoryRepository : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    using ProductId = int;
    using QObject::QObject;

    enum class Error
    {
        FailedToAddNewPrice,
        FailedToFetchPriceHistory,
        UnknownError
    };
    Q_ENUM(Error)

    virtual ~PriceHistoryRepository() = default;

    virtual void addNewPrice(const QVariantMap &newPrice) noexcept = 0;

    virtual void fetchPriceHistoryBy(ProductId productId) noexcept = 0;

signals:
    void added(const QByteArray &data);
    void fetched(const QByteArray &data);

    void errorOccurred(Error error, const QString &errorString);
};

}

namespace PriceHistoryForeign
{
Q_NAMESPACE
QML_NAMED_ELEMENT(PriceHistory)
QML_FOREIGN_NAMESPACE(Product::PriceHistoryRepository)
}

#endif // PRICEHISTORYREPOSITORY_H
