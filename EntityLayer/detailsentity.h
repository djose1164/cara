#ifndef DETAILSENTITY_H
#define DETAILSENTITY_H

#include "productentity.h"
#include <QObject>
#include <QtQml>

namespace EntityLayer {
class DetailsEntity : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int quantity READ quantity WRITE setQuantity NOTIFY quantityChanged)
    Q_PROPERTY(int total READ total NOTIFY totalChanged)
    Q_PROPERTY(ProductEntity *product READ product WRITE setProduct NOTIFY productChanged)

public:
    explicit DetailsEntity(QObject *parent = nullptr);
    DetailsEntity(const DetailsEntity& other, QObject *parent = nullptr);
    Q_INVOKABLE DetailsEntity &operator=(const DetailsEntity& other);

    int quantity() const;
    int total() const;

    ProductEntity *product() const;

signals:
    void quantityChanged();
    void totalChanged();
    void productChanged();

public slots:
    void setProduct(ProductEntity *product);
    void setQuantity(int quantity);

private:
    int quantity_;

    ProductEntity *product_;
};
}

#endif // DETAILSENTITY_H
