#include "iproductrepository.h"
#include "qnamsingleton.h"
#include "networkrequestsingleton.h"

namespace ProductInventory
{

IProductRepository::IProductRepository(QObject *parent)
    : QObject{parent}
    , path_{}
    , manager_{QNAMSingleton::qram()}
    , api_{Network::NetworkRequestSingleton::instance().api()}
{
}

IProductRepository::~IProductRepository() = default;

QString ProductInventory::IProductRepository::path() const
{
    return path_;
}

void ProductInventory::IProductRepository::setPath(const QString &newPath)
{
    if (path_ == newPath)
        return;
    path_ = newPath;
    emit pathChanged();
}

}
