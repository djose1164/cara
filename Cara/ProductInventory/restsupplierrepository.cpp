#include "restsupplierrepository.h"
#include "networkrequestsingleton.h"
#include "qnamsingleton.h"
#include <QRestReply>
#include <QJsonObject>
#include <QJsonArray>

using namespace Qt::StringLiterals;

constexpr auto suppliersField{"providers"_L1};

namespace ProductInventory
{

RestSupplierRepository::RestSupplierRepository(QObject *parent)
    : AbstractSupplierRepository{parent}
    , api_{Network::NetworkRequestSingleton::instance().api()}
    , manager_{QNAMSingleton::instance().qram()}
    , path_{"providers/"_L1}
{
}

void RestSupplierRepository::fetchSuppliers()
{
    auto req = api_->createRequest(path_);

    manager_->get(req, this, [this](QRestReply &reply) {
        if (not reply.isSuccess())
        {
            emit errorOccurred(reply.errorString());
            return;
        }

        if (const auto json{reply.readJson()};
            json and json->isObject() and json->object().contains(suppliersField))
        {
            QJsonArray supplierArr = (*json)[suppliersField].toArray();
            QList<Supplier> supplierList;
            std::ranges::for_each(supplierArr, [&](const auto json) {
                supplierList.append(Supplier::fromJson(json.toObject()));
            });

            emit suppliersFetched(supplierList);
        }
    });
}

} // namespace ProductInventory
