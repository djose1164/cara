#ifndef ADDRESS_H
#define ADDRESS_H

#include <QObject>
#include <QQmlEngine>

namespace Entity
{
class Address : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int addressId READ addressId WRITE setAddressId NOTIFY addressIdChanged FINAL)
    Q_PROPERTY(int houseNumber READ houseNumber WRITE setHouseNumber NOTIFY houseNumberChanged FINAL)
    Q_PROPERTY(int countryId READ countryId WRITE setCountryId NOTIFY countryIdChanged FINAL)
    Q_PROPERTY(int provinceId READ provinceId WRITE setProvinceId NOTIFY provinceIdChanged FINAL)
    Q_PROPERTY(int municipalityId READ municipalityId WRITE setMunicipalityId NOTIFY municipalityIdChanged FINAL)
    Q_PROPERTY(int sectorId READ sectorId WRITE setSectorId NOTIFY sectorIdChanged FINAL)
    Q_PROPERTY(QString street READ street WRITE setStreet NOTIFY streetChanged FINAL)
public:
    static Address *fromMap(const QByteArray &data);

    explicit Address(QObject *parent = nullptr);

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

signals:
    void addressIdChanged();
    void houseNumberChanged();

    void countryIdChanged();

    void provinceIdChanged();

    void municipalityIdChanged();

    void sectorIdChanged();

    void streetChanged();

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
