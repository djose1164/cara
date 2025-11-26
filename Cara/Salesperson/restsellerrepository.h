#ifndef RESTSELLERREPOSITORY_H
#define RESTSELLERREPOSITORY_H

#include "abstractsellerrepository.h"

#include <QNetworkRequestFactory>
#include <QRestAccessManager>

namespace Seller
{

class RestSellerRepository : public AbstractSellerRepository
{
    Q_OBJECT
public:
    using AdminId = int;

    RestSellerRepository(QObject *parent = nullptr);

    void fetchSellers() override;

    AdminId adminId() const;
    void setAdminId(AdminId newAdminId);

private:
    std::shared_ptr<QNetworkRequestFactory> api_;
    std::unique_ptr<QRestAccessManager> manager_;
    const QString path_;
    AdminId adminId_;
};

} // namespace Seller

#endif // RESTSELLERREPOSITORY_H
