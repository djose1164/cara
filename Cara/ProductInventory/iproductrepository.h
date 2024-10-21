#ifndef IPRODUCTREPOSITORY_H
#define IPRODUCTREPOSITORY_H

#include "productentity.h"
#include <QObject>


namespace ProductInventory
{

class IProductRepository : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged FINAL)
    QML_ANONYMOUS
public:
    enum class Error
    {
        FailedToFetch,
        FailedToUpdate,
        UnknownError
    };

    using ProductId = int;

    explicit IProductRepository(QObject *parent = nullptr);

    virtual ~IProductRepository();

    virtual void add(const EntityLayer::Product &product) = 0;

    virtual void update() = 0;

    virtual void remove(ProductId productId) = 0;

    QString path() const;
    void setPath(const QString &newPath);

signals:
    void added(const EntityLayer::Product &product);

    void fetched(const QList<EntityLayer::Product> &products);

    void errorOccurred(Error error, const QString &errorString);

    void pathChanged();

protected:
    std::unique_ptr<QRestAccessManager> manager_;
    std::shared_ptr<QNetworkRequestFactory> api_;

private:
    QString path_;
};

}

#endif // IPRODUCTREPOSITORY_H
