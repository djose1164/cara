#ifndef INVENTORYREPOSITORY_H
#define INVENTORYREPOSITORY_H

#include "iinventoryrepository.h"

#include <QQmlEngine>

namespace Inventory {
class InventoryRepository : public QObject, public IInventoryRepository
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit InventoryRepository(QObject *parent = nullptr);

public slots:
    void fetchAll() override;

    void update(const QVariantMap &stocks) override;

signals:


};
}

#endif // INVENTORYREPOSITORY_H
