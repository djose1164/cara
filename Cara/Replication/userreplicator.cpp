#include "userreplicator.h"
#include <QDebug>
#include <QJsonObject>
#include <QSqlError>
#include <QSqlQuery>

using namespace Qt::StringLiterals;

constexpr auto INSERT{"insert into user(username, forename, surname, admin_id) "
                      "values(:username, :forename, :surname, :adminId)"_L1};
constexpr auto TRUNCATE{"delete from user"_L1};
constexpr auto usernameField{"username"_L1};
constexpr auto forenameField{"forename"_L1};
constexpr auto surnameField{"surname"_L1};
constexpr auto adminIdField{"adminId"_L1};

constexpr auto usernameBind{":username"_L1};
constexpr auto forenameBind{":forename"_L1};
constexpr auto surnameBind{":surname"_L1};
constexpr auto adminIdBind{":adminId"_L1};


namespace Replication
{

UserReplicator::UserReplicator(QObject *parent)
    : EntityReplicator{parent}
{
}

void UserReplicator::apply(const QJsonObject &payload)
{
    if (not isPayloadValid(payload))
    {
        const auto errstr{"Cannot apply replicate due to invalid payload"_L1};
        qWarning() << errstr;
        emit errorOccurred(errstr);
        return;
    }

    QSqlQuery query;
    query.exec(TRUNCATE);
    if (not query.prepare(INSERT))
    {
        const auto errstr{"Failed to prepare query: %1"_L1.arg(query.lastError().text())};
        qWarning() << errstr;
        emit errorOccurred(errstr);
        return;
    }
    query.bindValue(usernameBind, payload[usernameField]);
    query.bindValue(forenameBind, payload[forenameField]);
    query.bindValue(surnameBind, payload[surnameField]);
    query.bindValue(adminIdBind, payload[adminIdField].toInt());

    if (not query.exec())
    {
        const auto errstr{"Failed to execute query: %1"_L1.arg(query.lastError().text())};
        qWarning() << errstr;
        emit errorOccurred(errstr);
        return;
    }

    emit finished();
}

QString UserReplicator::entityName() const
{
    return "user"_L1;
}

bool UserReplicator::isPayloadValid(const QJsonObject &payload)
{
    return payload.contains("username"_L1)
           and payload.contains("adminId"_L1)
           and payload.contains("forename"_L1)
           and payload.contains("surname"_L1);
}

} // namespace Replication
