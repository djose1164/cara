#include "salespersoncontroller.h"
#include "data/salespersonmanager.h"

namespace Controller
{
SalespersonController::SalespersonController(QObject *parent)
    : QObject{parent}
    , manageable_{new Data::SalespersonManager(this)}
    , adminId_{}
    , salespersonId_{}
    , buffer_{}
{
    connect(manageable_, &Business::Manageable::finished, this, [&] {
        setBuffer(manageable_->buffer());
    });

    connect(manageable_, &Business::Manageable::fetched, this, &SalespersonController::fetched);

    connect(manageable_, &Business::Manageable::errorOccurred, this, &SalespersonController::errorOccurred);
}

int SalespersonController::adminId() const
{
    return adminId_;
}

void SalespersonController::setAdminId(SalespersonId newAdminId)
{
    if (adminId_ == newAdminId)
        return;
    adminId_ = newAdminId;
    emit adminIdChanged();
}

void SalespersonController::readSalespeople()
{
    Q_ASSERT(adminId_ > 0);

    manageable_->readAll(adminId_);
}

void SalespersonController::readSalesperson()
{
    Q_ASSERT(salespersonId_ > 0);

    manageable_->read(salespersonId_);
}

void SalespersonController::update()
{

}

int SalespersonController::salespersonId() const
{
    return salespersonId_;
}

void SalespersonController::setSalespersonId(SalespersonId newSalespersonId)
{
    if (salespersonId_ == newSalespersonId)
        return;
    salespersonId_ = newSalespersonId;
    emit salespersonIdChanged();
}

QByteArray SalespersonController::buffer() const
{
    return buffer_;
}

void SalespersonController::setBuffer(const QByteArray &newBuffer)
{
    if (buffer_ == newBuffer)
        return;
    buffer_ = newBuffer;
    emit bufferChanged();
}
}
