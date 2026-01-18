#include "bootstrapreplicationservice.h"
#include "replicationengine.h"
#include "userreplicator.h"

using namespace Qt::StringLiterals;

namespace Replication
{

BootstrapReplicationService::BootstrapReplicationService(QObject *parent)
    : QObject{parent}
    , engine_{new ReplicationEngine(this)}
{
    engine_->registerReplicator(new UserReplicator);

    connect(engine_,
            &AbstractReplicationEngine::finished,
            this,
            &BootstrapReplicationService::finished);
}

void BootstrapReplicationService::replicateForUser(const QVariantMap &user)
{
    qDebug() << "BootstrapReplicationService::replicateForUser: replicating user...";
    ReplicationMessage msg = {.entity = "user"_L1, .payload = QJsonObject::fromVariantMap(user)};
    engine_->run(QList{msg});
}

void BootstrapReplicationService::replicateGlobalData()
{
}

} // namespace Replication
