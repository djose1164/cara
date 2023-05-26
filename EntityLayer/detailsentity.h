#ifndef DETAILSENTITY_H
#define DETAILSENTITY_H

#include "productentity.h"
#include "jsonexportable.h"
#include <QObject>
#include <QtQml>

namespace EntityLayer {
class DetailsEntity : public QObject, public JsonExportable
{

public:
    explicit DetailsEntity(QObject *parent = nullptr);

    void setQuantity(int quantity);
    int quantity() const;

    int total() const;
    QBindable<int> bindableTotal() const;

    void setProduct(ProductEntity *const product);
    ProductEntity *product() const;

    QByteArray toJson() const override;

private:
    QProperty<int> quantity_;
    QProperty<int> total_;

    ProductEntity *product_;
};
}

#endif // DETAILSENTITY_H
