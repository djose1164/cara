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

Address::Address()
    : addressId_{}
    , houseNumber_{}
    , countryId_{}
    , provinceId_{}
    , municipalityId_{}
    , sectorId_{}
    , street_{}
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
}
}
