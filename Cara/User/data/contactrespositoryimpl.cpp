#include "contactrespositoryimpl.h"
#include "QNAMSingleton.h"
#include "networkrequestsingleton.h"
#include <QRestAccessManager.h>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto path{"contacts"_L1};
}

namespace Data {
ContactRespositoryImpl::ContactRespositoryImpl(QObject *parent)
    : QObject{parent}
    , manager_{new QRestAccessManager{QNAMSingleton::instance().qnam()}}
    , api_{Utils::NetworkRequestSingleton::instance().api()}
{

}

void ContactRespositoryImpl::getById(int id, Handler &&h)
{

}

void ContactRespositoryImpl::update(const QVariantMap &data, const QUrlQuery &query, Handler &&h)
{
    manager_->put(api_->createRequest(path, query), data, this, h);
}
}
