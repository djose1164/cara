#ifndef DETAILSENTITY_H
#define DETAILSENTITY_H

#include "productentity.h"
#include <QObject>
#include <QtQml>

namespace EntityLayer {
class DetailsEntity : public QObject
{

public:
    explicit DetailsEntity(QObject *parent = nullptr);

    void setQuantity(int quantity);
    int quantity() const;

    int total() const;
    QBindable<int> bindableTotal() const;
    
    void setProduct(const Product &product);
    const Product &product() const;

    QByteArray toJson() const;

signals:
    void productChanged();

private:
    QProperty<int> quantity_;
    QProperty<int> total_;
    
    QProperty<Product> product_;
};
}

#endif // DETAILSENTITY_H
