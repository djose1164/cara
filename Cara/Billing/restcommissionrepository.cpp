#include "restcommissionrepository.h"
#include "Network/qnamsingleton.h"
#include "Network/networkrequestsingleton.h"
#include "commission.h"

#include <QJsonArray>
#include <QRestAccessManager>
#include <QRestReply>
#include <QJsonDocument>
#include <QJsonObject>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto commissionsField{"commissions"_L1};
constexpr auto resourceIdentificator{"/%1"_L1};
}

namespace Billing
{

RestCommissionRepository::RestCommissionRepository(QObject *parent)
    : CommissionRepository{parent}
    , manager_{new QRestAccessManager(QNAMSingleton::instance().qnam())}
    , api_{Network::NetworkRequestSingleton::instance().api()}
    , path_{"commissions"_L1}
{
}

void RestCommissionRepository::fetchCommissionsBySellerId(int spId)
{
    fetchCommissions();
}

void RestCommissionRepository::fetchCommissions()
{
    manager_->get(api_->createRequest(path_),
                  this,
                  [this](QRestReply &reply)
                  {
                      if (!reply.isSuccess())
                      {
                          emit errorOccurred(Error::FetchFailure, reply.errorString());
                          return;
                      }

                      if (const auto json{reply.readJson()};
                          json and json->isObject() and json->object().contains(commissionsField))
                      {
                          const auto commissionsArray = (*json)[commissionsField].toArray();
                          QList<Commission> commissionList;
                          std::ranges::for_each(commissionsArray,
                                                [&commissionList](const auto &jsonValue)
                                                {
                                                    commissionList.append(Commission::fromJson(jsonValue.toObject()));
                                                });
                          emit fetched(commissionList);
                      }
                      else
                      {
                          qWarning() << json;
                          qWarning() << "json is not object or doesn't contain "_ba + commissionsField;
                          emit errorOccurred(Error::FetchFailure, "json is not object or doesn't contain 'commissions'"_L1);
                      }
                  });
}

void RestCommissionRepository::addCommission(const Commission &commission)
{
    qDebug() << commission.toJson();
    qDebug() << QJsonDocument{commission.toJson()};
    manager_->post(api_->createRequest(path_),
                   QJsonDocument{commission.toJson()},
                   this,
                   [this](QRestReply &reply)
                    {
                        if (!reply.isSuccess())
                        {
                            emit errorOccurred(Error::AddFailure, reply.errorString());
                            return;
                        }

                        if (const auto json{reply.readJson()};
                                json and json->isObject() and json->object().contains("commission"_L1))
                        {
                            emit added(Commission::fromJson((*json)["commission"_L1].toObject()));
                        }
                    });
}

void RestCommissionRepository::patchCommission(const Commission &commission, std::function<void()> callback)
{
    if (not commission.isPatchable())
    {
        emit errorOccurred(Error::PatchFailure,
                            "The commission is not valid to be patched."_L1);
        return;
    }

    const QString resourceId{resourceIdentificator
                .arg(QString::number(commission.commissionId()))};
    manager_->patch(api_->createRequest(path_ + resourceId),
                    QJsonDocument{commission.toJson()},
                    this,
                    [this,cb = std::move(callback)](QRestReply &reply)
                    {
                        if (not reply.isSuccess())
                        {
                            emit errorOccurred(Error::PatchFailure, reply.errorString());
                            return;
                        }
                        qDebug() << "RestCommissionRepository::patchCommission: calling callback";
                        if (cb)
                            cb();
                    });
}

bool RestCommissionRepository::supportsServerSideCalculation() const
{
    return true;
}

} // namespace Billing
