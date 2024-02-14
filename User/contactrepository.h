#ifndef CONTACTREPOSITORY_H
#define CONTACTREPOSITORY_H

#include <QObject>

class ContactRepository
{
public:
    virtual ~ContactRepository();

    virtual void getById(int id) = 0;

    virtual void update(const QVariantMap &data) = 0;
};

Q_DECLARE_INTERFACE(ContactRepository, "Cara.ContactRepositoryInterface")

#endif // CONTACTREPOSITORY_H
