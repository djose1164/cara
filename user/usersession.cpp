#include "usersession.h"
#include "associatesalespersonservice.h"
#include "localuserstore.h"
#include <QJsonDocument>
#include <QJsonObject>

UserSession::UserSession(QObject *parent)
    : QObject{parent}
{

}

Entity::UserEntity UserSession::user() const
{
    return user_;
}

void UserSession::setUser(const Entity::UserEntity &newUser)
{
    if (user_ == newUser)
        return;
    user_ = newUser;
    emit userChanged();
}

void UserSession::buildUser(const QByteArray &data)
{
    qDebug() << "UserSession::buildUser: " << data;
    setUser(Entity::UserEntity::fromJson(data));

    qDebug() << "UserSession::buildUser: " << user_.userId() << " " << user_.username();
}

void UserSession::update()
{
    qDebug() << "UserSession::update: updating...";
    const Business::LocalUserStore userStore{};
    Q_ASSERT(int(userStore.user().userType()));
    qDebug() << "  " << userStore.user().userType() << " " << userStore.user().username();

    setUser(userStore.user());
    if (user_.isSalesperson())
    {
        const auto salespersonService{new Salesperson::AssociateSalespersonService};
        salespersonService->fetchAssociateSalespersonById(user_.userId());
        connect(salespersonService, &Salesperson::AssociateSalespersonService::fetched, this, [=, this] {
            const QJsonDocument doc{QJsonDocument::fromJson(salespersonService->data())};
            Entity::Salesperson::fromMap(doc.object().toVariantMap());

            salespersonService->deleteLater();
        });
    }
    qDebug() << "UserSession::update: " << user_.userType();
}

void UserSession::clear()
{
    qDebug() << "UserSession::clear: clearing session.";
    user_.clear();
    emit userChanged();
}

Entity::Salesperson UserSession::salesperson() const
{
    return salesperson_;
}

void UserSession::setSalesperson(const Entity::Salesperson &newSalesperson)
{
    if (salesperson_ == newSalesperson)
        return;
    salesperson_ = newSalesperson;
    emit salespersonChanged();
}

Session *UserSession::session()
{
    return this;
}
