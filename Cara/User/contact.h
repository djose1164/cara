#ifndef CONTACT_H
#define CONTACT_H

#include "address.h"
#include <QObject>
#include <QQmlEngine>

namespace Entity
{
class Contact : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int contactId READ contactId WRITE setContactId NOTIFY contactIdChanged FINAL)
    Q_PROPERTY(QString telephone READ telephone WRITE setTelephone NOTIFY telephoneChanged FINAL)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged FINAL)
    Q_PROPERTY(Address *address READ address WRITE setAddress NOTIFY addressChanged FINAL)
public:
    static Contact *fromMap(const QByteArray &data);

    explicit Contact(QObject *parent = nullptr);

    int contactId() const;
    void setContactId(int newContactId);

    QString telephone() const;
    void setTelephone(const QString &newTelephone);

    QString email() const;
    void setEmail(const QString &newEmail);

    Address *address();
    void setAddress(Address *newAddress);

signals:
    void contactIdChanged();
    void telephoneChanged();
    void emailChanged();
    void addressChanged();

private:
    int contactId_;
    QString telephone_;
    QString email_;
    Address *address_;
};
};

#endif // CONTACT_H
