#ifndef BILLBUSINESS_H
#define BILLBUSINESS_H

#include "billmodel.h"
#include "rest.h"
#include <QObject>
#include <QtQml>

namespace BusinessLayer {
using ModelLayer::BillModel, EntityLayer::UserEntity;

class BillBusiness : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(BillModel *model READ model WRITE setModel NOTIFY modelChanged BINDABLE bindableModel)
    Q_PROPERTY(UserEntity *user READ user WRITE setUser NOTIFY userChanged BINDABLE bindableUser)

public:
    explicit BillBusiness(QObject *parent = nullptr);

    BillModel *model() const;
    void setModel(BillModel *const model);
    QBindable<BillModel *> bindableModel();

    UserEntity *user() const;
    void setUser(UserEntity *const user);
    QBindable<UserEntity *> bindableUser();

public slots:
    void fetchAllCustomers();
    void fetchCustomerById(int customerId);
    void fetchCurrentCustomer();

    void clearModel();

signals:
    void modelChanged();
    void userChanged();

    void allCustomersFetched(const QVariantMap &resp);

    void completed();

private:
    QProperty<Rest *> rest_;

    Q_OBJECT_BINDABLE_PROPERTY(BillBusiness, BillModel *, model_, &BillBusiness::modelChanged)
    Q_OBJECT_BINDABLE_PROPERTY(BillBusiness, UserEntity *, user_, &BillBusiness::userChanged)
};
}

#endif // BILLBUSINESS_H
