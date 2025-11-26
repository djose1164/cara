#include "restsellerrepository.h"
#include "networkrequestsingleton.h"
#include "qnamsingleton.h"
#include "salespersoncontroller.h"

using namespace Qt::StringLiterals;

namespace Seller
{

RestSellerRepository::RestSellerRepository(QObject *parent)
    : AbstractSellerRepository{parent}
    , api_{Network::NetworkRequestSingleton::instance().api()}
    , manager_{QNAMSingleton::instance().qram()}
    , path_{"salesperson"}
    , adminId_{-1}
{
}

void RestSellerRepository::fetchSellers()
{
    if (adminId_ == -1)
    {
        qDebug() << "RestSellerRepository::fetchSellers: invalid adminId";
        return;
    }

    auto *spController{new Salesperson::SalespersonController(this)};
    spController->setAdminId(adminId_);
    spController->readSalespeople();

    connect(spController,
            &Salesperson::SalespersonController::fetched,
            this,
            [this, spController](const auto &sellerList) {
                spController->deleteLater();
                qDebug() << QVariant::fromValue(sellerList);
                emit fetched(sellerList);
            });

}

RestSellerRepository::AdminId RestSellerRepository::adminId() const
{
    return adminId_;
}

void RestSellerRepository::setAdminId(AdminId newAdminId)
{
    adminId_ = newAdminId;
}

} // namespace Seller
