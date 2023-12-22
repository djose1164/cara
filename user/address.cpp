#include "address.h"
#include <QJsonDocument>
#include <QJsonObject>

namespace Entity
{
Address *Address::fromMap(const QByteArray &data)
{
    const auto addressData{QJsonDocument::fromJson(data).object()};
    auto address{new Address};

    address->setAddressId(addressData["id"].toInt());
    address->setHouseNumber(addressData["house_number"].toInt());
    return address;
}

Address::Address(QObject *parent)
    : QObject{parent}
{

}

int Address::addressId() const
{
    return addressId_;
}

void Address::setAddressId(int newAddressId)
{
    if (addressId_ == newAddressId)
        return;
    addressId_ = newAddressId;
    emit addressIdChanged();
}

int Address::houseNumber() const
{
    return houseNumber_;
}

void Address::setHouseNumber(int newHouseNumber)
{
    if (houseNumber_ == newHouseNumber)
        return;
    houseNumber_ = newHouseNumber;
    emit houseNumberChanged();
}

int Address::countryId() const
{
    return countryId_;
}

void Address::setCountryId(int newCountryId)
{
    if (countryId_ == newCountryId)
        return;
    countryId_ = newCountryId;
    emit countryIdChanged();
}

int Address::provinceId() const
{
    return provinceId_;
}

void Address::setProvinceId(int newProvinceId)
{
    if (provinceId_ == newProvinceId)
        return;
    provinceId_ = newProvinceId;
    emit provinceIdChanged();
}

int Address::municipalityId() const
{
    return municipalityId_;
}

void Address::setMunicipalityId(int newMunicipalityId)
{
    if (municipalityId_ == newMunicipalityId)
        return;
    municipalityId_ = newMunicipalityId;
    emit municipalityIdChanged();
}

int Address::sectorId() const
{
    return sectorId_;
}

void Address::setSectorId(int newSectorId)
{
    if (sectorId_ == newSectorId)
        return;
    sectorId_ = newSectorId;
    emit sectorIdChanged();
}

QString Address::street() const
{
    return street_;
}

void Address::setStreet(const QString &newStreet)
{
    if (street_ == newStreet)
        return;
    street_ = newStreet;
    emit streetChanged();
}
}
