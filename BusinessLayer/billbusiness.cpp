#include "billbusiness.h"
#include "constants.h"
#include "currentuser.h"

using BusinessLayer::BillBusiness, EntityLayer::UserEntity;

BillBusiness::BillBusiness(QObject *parent)
    : QObject{parent}
    , rest_{new Rest(this)}
    , model_{nullptr}
    , user_{new UserEntity(this)}
{
    const QString base{Constants::instance()->base()};
    rest_->base(base + "/customers");

    QSettings settings{};
    const QString token{settings.value("Credentials/refreshToken").toString()};
    rest_->refreshToken(token);
    rest_->refreshAccessToken();
}

ModelLayer::BillModel *BillBusiness::model() const
{
    return model_.value();
}

void BillBusiness::setModel(BillModel * const model)
{
    model_ = model;
}

QBindable<ModelLayer::BillModel *> BillBusiness::bindableModel()
{
    return &model_;
}

UserEntity *BillBusiness::user() const
{
    return user_.value();
}

void BillBusiness::setUser(UserEntity * const user)
{
    user_ = user;
}

QBindable<UserEntity *> BillBusiness::bindableUser()
{
    return &user_;
}

void BillBusiness::fetchAllCustomers()
{
    if (!model_)
    {
        qWarning() << "BillBusiness::fetchAllCustomers: No model set.";
        return;
    }

    using UserTypes = UserEntity::UserTypes;
    Q_ASSERT(user_->userType() == UserTypes::ADMIN);

    rest_->get("/");

    connect(rest_, &Rest::succeeded, this, [this](const QVariantMap &resp) {
        const auto customers{resp.value("customers").toList()};
        // TODO: Add to necessary entites a static fromJson method that creates
        // a new entity.
        qDebug() << resp;
        for (const auto &customer : customers)
        {
            const auto customerMap{customer.toMap()};

            auto bill{new EntityLayer::BillEntity(model_)};
            const auto user{bill->user()};
            user->setCustomerId(customerMap["customer_id"].toInt());

            const auto orders{customerMap["orders"].toList()};
            for (const auto &order : orders)
            {
                const auto map{order.toMap()};

                const auto strDate{map["date"].toString()};
                Q_ASSERT(!strDate.isEmpty());
                qDebug() << strDate;

                bill->setDate(QDate::fromString(strDate, "dd/MM/yyyy"));
                Q_ASSERT(!bill->date().isNull());
                qDebug() << "-*-*-*-*-*-*-*-*" << strDate;

                bill->setOrderId(map["orderId"].toInt());

                const auto paymentMap{map["payment"].toMap()};
                const auto payment{bill->payment()};
                payment->setAmountToPay(paymentMap["amount_to_pay"].toInt());
                payment->setPaidAmount(paymentMap["paid_amound"].toInt());

                const auto details{map["order_details"].toList()};
                for (const auto &detail : details)
                {
                    const auto map{detail.toMap()};
                    auto newDetail{new EntityLayer::DetailsEntity};
                    newDetail->setQuantity(map["quantity"].toInt());

                    const auto productMap{map["product"].toMap()};
                    const auto product{newDetail->product()};
                    product->setName(productMap["name"].toString());
                    product->setImageUrl(productMap["image_url"].toString());
                    product->setPrice(productMap["sell_price"].toInt());

                    bill->appendDetail(newDetail);
                }
            }

            model_->append(bill);
        }
        emit allCustomersFetched(resp);
    });
}

void BillBusiness::fetchCustomerById(int customerId)
{
    Q_ASSERT(customerId);

    qDebug() << "BillBusiness::fetchCustomerById " << customerId;

    rest_->get(QString("/%1").arg(customerId));

    connect(rest_, &Rest::succeeded, this, [this, customerId](const QVariantMap &resp) {
        const auto customer{resp["customer"].toMap()};

        for (const auto &order : customer["orders"].toList())
        {
            auto bill{new EntityLayer::BillEntity};
            const auto user{bill->user()};
            user->setCustomerId(customerId);

            const auto map{order.toMap()};

            const auto strDate{map["date"].toString()};
            bill->setDate(QDate::fromString(strDate, "dd/MM/yyyy"));

            bill->setOrderId(map["id"].toInt());

            const auto paymentMap{map["payment"].toMap()};
            const auto payment{bill->payment()};
            payment->setAmountToPay(paymentMap["amount_to_pay"].toInt());
            payment->setPaidAmount(paymentMap["paid_amound"].toInt());

            for (const auto &detail : map["order_details"].toList())
            {
                const auto map{detail.toMap()};
                auto newDetail{new EntityLayer::DetailsEntity};
                newDetail->setQuantity(map["quantity"].toInt());

                const auto productMap{map["product"].toMap()};
                const auto product{newDetail->product()};
                product->setName(productMap["name"].toString());
                product->setImageUrl(productMap["image_url"].toString());
                product->setPrice(productMap["sell_price"].toInt());

                bill->appendDetail(newDetail);
            }

            model_->append(bill);
        }
    });
}

void BillBusiness::fetchCurrentCustomer()
{
    const auto currentUser{CurrentUser::instance()};
    fetchCustomerById(currentUser->customerId());
}

void BillBusiness::clearModel()
{
    model_->clear();
}


