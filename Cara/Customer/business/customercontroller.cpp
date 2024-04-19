#include "customercontroller.h"
#include <QJsonObject>

namespace Controller
{
CustomerController::CustomerController(QObject *parent)
    : QObject{parent}
{}

Entity::Customer CustomerController::fromMap(const QVariantMap &data) const
{
    return Entity::Customer::fromJson(QJsonObject::fromVariantMap(data));
}
}
