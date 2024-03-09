#include "organizationresource.h"
#include "utils.h"

#include <QRestReply>
#include <QRestAccessManager>
#include <QJsonDocument>

using namespace Qt::StringLiterals;

static constexpr auto resourceId{"/%1"_L1};

namespace Shopping
{
OrganizationResource::OrganizationResource(QObject *parent)
    : AbstractResource{parent}
    , path_{}
{}

void OrganizationResource::readAll()
{
    manager_->get(api_->createRequest(path_),
                  this, &OrganizationResource::handleFinished);
}

void OrganizationResource::readById(OrganizationId id)
{
    manager_->get(api_->createRequest(path_ + resourceId.arg(QString::number(id))),
                  this, &OrganizationResource::handleFinished);
}

void OrganizationResource::handleFinished(QRestReply &reply)
{
    if (!reply.isSuccess())
    {
        if (Utils::isOperationCanceledError(reply))
            emit timeout();
        emit errorOccurred(reply.errorString());
        return;
    }

    if (const auto doc{reply.readJson()})
        emit fetched(doc->toJson());
}
}
