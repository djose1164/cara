#ifndef ADDRESS_H
#define ADDRESS_H

#include <QObject>
#include <QQmlEngine>

namespace Entity
{
class Address
{
    Q_GADGET
    Q_PROPERTY(int addressId READ addressId WRITE setAddressId FINAL)
    Q_PROPERTY(int houseNumber READ houseNumber WRITE setHouseNumber FINAL)
    Q_PROPERTY(int countryId READ countryId WRITE setCountryId FINAL)
    Q_PROPERTY(int provinceId READ provinceId WRITE setProvinceId FINAL)
    Q_PROPERTY(int municipalityId READ municipalityId WRITE setMunicipalityId FINAL)
    Q_PROPERTY(int sectorId READ sectorId WRITE setSectorId FINAL)
    Q_PROPERTY(QString street READ street WRITE setStreet FINAL)
    QML_VALUE_TYPE(address)
    QML_CONSTRUCTIBLE_VALUE
public:
    static Address *fromMap(const QByteArray &data);

    Address();

    int addressId() const;
    void setAddressId(int newAddressId);

    int houseNumber() const;
    void setHouseNumber(int newHouseNumber);

    int countryId() const;
    void setCountryId(int newCountryId);

    int provinceId() const;
    void setProvinceId(int newProvinceId);

    int municipalityId() const;
    void setMunicipalityId(int newMunicipalityId);

    int sectorId() const;
    void setSectorId(int newSectorId);

    QString street() const;
    void setStreet(const QString &newStreet);

private:
    int addressId_;
    int houseNumber_;
    int countryId_;
    int provinceId_;
    int municipalityId_;
    int sectorId_;
    QString street_;
};
}

#endif // ADDRESS_H
