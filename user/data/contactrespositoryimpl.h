#ifndef CONTACTRESPOSITORYIMPL_H
#define CONTACTRESPOSITORYIMPL_H

#include "contactrepository.h"
#include <QObject>

namespace Data {
class ContactRespositoryImpl : public QObject, public ContactRepository
{
    Q_OBJECT
    Q_INTERFACES(ContactRepository)
public:
    explicit ContactRespositoryImpl(QObject *parent = nullptr);

    void getById(int id) override;
    void update(const QVariantMap &data) override;
signals:


};
}

#endif // CONTACTRESPOSITORYIMPL_H
