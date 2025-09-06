#ifndef SQLITESELLERREPOSITORY_H
#define SQLITESELLERREPOSITORY_H

#include "abstractsellerrepository.h"
#include <QQmlEngine>

namespace Seller
{

class SQLiteSellerRepository : public AbstractSellerRepository
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit SQLiteSellerRepository(QObject *parent = nullptr);

    void fetchSellers() override;
};

} // namespace Seller

#endif // SQLITESELLERREPOSITORY_H
