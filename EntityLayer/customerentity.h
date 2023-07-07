#ifndef CUSTOMERENTITY_H
#define CUSTOMERENTITY_H

#include "billmodel.h"
#include "userentity.h"
#include <QObject>

namespace EntityLayer {
using ModelLayer::BillModel;

class CustomerEntity : public UserEntity
{
    Q_OBJECT
public:
    explicit CustomerEntity(QObject *parent = nullptr);

    void setBills(BillModel *const bills);
    BillModel *bills() const;

    int adminId() const;
    void setAdminId(int adminId);
    QBindable<int> bindableAdminId();

private:
    BillModel *bills_;
    QProperty<int> adminId_;
};
}

#endif // CUSTOMERENTITY_H
