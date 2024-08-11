#ifndef DETAILSENTITY_H
#define DETAILSENTITY_H

#include "productentity.h"
#include <QObject>
#include <QtQml>
#include <QBindable>

namespace EntityLayer {
class DetailsEntity : public QObject
{
public:
    static DetailsEntity *fromJson(const QJsonObject &data);

    explicit DetailsEntity(QObject *parent = nullptr);

    void setQuantity(int quantity);
    int quantity() const;

    int price() const;

    int total() const;
    QBindable<int> bindableTotal() const;
    
    void setProduct(const Product &product);
    const Product &product() const;

    QByteArray toJson() const;

signals:
    void productChanged();

private:
    QProperty<int> quantity_;
    QProperty<int> price_;
    QProperty<int> total_;
    
    QProperty<Product> product_;
};
}

#endif // DETAILSENTITY_H
