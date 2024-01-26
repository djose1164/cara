#include "paymentcontroller.h"
#include <QJsonObject>

namespace Controller
{
PaymentController::PaymentController(QObject *parent)
    : QObject{parent}
{}

Entity::Payment PaymentController::fromJsonObject(const QJsonObject &data) const
{
    return Entity::Payment::fromJsonObject(data);
}
}
