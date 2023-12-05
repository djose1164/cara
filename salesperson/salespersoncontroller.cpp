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
    connect(manageable_, &Business::Manageable::fetched, this, [&] {
        setBuffer(manageable_->buffer());
        emit fetched();
    });
}

int SalespersonController::adminId() const
{
    return adminId_;
}

void SalespersonController::setAdminId(int newAdminId)
{
    if (adminId_ == newAdminId)
        return;
    adminId_ = newAdminId;
    emit adminIdChanged();
}

void SalespersonController::readAll()
{
    Q_ASSERT(adminId_ > 0);

    manageable_->readAll(adminId_);
}

void SalespersonController::read(int salespersonId)
{

}

void SalespersonController::update()
{

}

int SalespersonController::salespersonId() const
{
    return salespersonId_;
}

void SalespersonController::setSalespersonId(int newSalespersonId)
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
