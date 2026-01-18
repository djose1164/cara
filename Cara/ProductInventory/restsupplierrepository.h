#ifndef RESTSUPPLIERREPOSITORY_H
#define RESTSUPPLIERREPOSITORY_H

#include "abstractsupplierrepository.h"

#include <QQmlEngine>
#include <QNetworkRequestFactory>
#include <QRestAccessManager>

namespace ProductInventory
{

class RestSupplierRepository : public AbstractSupplierRepository
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit RestSupplierRepository(QObject *parent = nullptr);

    void fetchSuppliers() override;

private:
    std::shared_ptr<QNetworkRequestFactory> api_;
    std::unique_ptr<QRestAccessManager> manager_;
    const QString path_;
};

} // namespace ProductInventory

#endif // RESTSUPPLIERREPOSITORY_H
