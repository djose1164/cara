#include "contact.h"
#include <QJsonDocument>
#include <QJsonObject>

namespace Entity {
Contact *Contact::fromMap(const QByteArray &data)
{
    const auto contactData{QJsonDocument::fromJson(data).object()};
    const auto contact{new Contact};

    contact->setContactId(contactData["id"].toInt());

    const QJsonDocument addressDoc{contactData["address"].toObject()};
    const auto address{Address::fromMap(addressDoc.toJson())};
    contact->setAddress(address);

    return contact;
}

Contact::Contact(QObject *parent)
    : QObject{parent}
{

}

int Contact::contactId() const
{
    return contactId_;
}

void Contact::setContactId(int newContactId)
{
    if (contactId_ == newContactId)
        return;
    contactId_ = newContactId;
    emit contactIdChanged();
}

QString Contact::telephone() const
{
    return telephone_;
}

void Contact::setTelephone(const QString &newTelephone)
{
    if (telephone_ == newTelephone)
        return;
    telephone_ = newTelephone;
    emit telephoneChanged();
}

QString Contact::email() const
{
    return email_;
}

void Contact::setEmail(const QString &newEmail)
{
    if (email_ == newEmail)
        return;
    email_ = newEmail;
    emit emailChanged();
}

Address *Contact::address()
{
    return address_;
}

void Contact::setAddress(Address *newAddress)
{
    if (address_ == newAddress)
        return;
    address_ = newAddress;
    emit addressChanged();
}
}
