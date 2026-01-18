#include "replicationengine.h"
#include "entityreplicator.h"

#include <QJsonObject>

using namespace Qt::StringLiterals;

namespace Replication
{

ReplicationEngine::ReplicationEngine(QObject *parent)
    : AbstractReplicationEngine{parent}
{
}

ReplicationEngine::~ReplicationEngine()
{
    qDeleteAll(replicators_);
    replicators_.clear();
}

void ReplicationEngine::run(const ReplicationMessageList &plan)
{
    std::ranges::for_each(plan, [this](const auto &message) {
        if (replicators_.contains(message.entity))
        {
            const auto replicator{replicators_[message.entity]};
            replicator->apply(message.payload);
        }
        else
        {
            const auto errstr = "There's no replicator resgistered for %1."_L1.arg(message.entity);
            qWarning() << "ReplicationEngine::run: " << errstr;
            emit errorOccurred(errstr);
            return;
        }
    });

    emit finished();
}

void ReplicationEngine::registerReplicator(EntityReplicator *r)
{
    if (!r)
        return;
    r->setParent(this);
    replicators_.insert(r->entityName(), r);
}

std::optional<EntityReplicator *> ReplicationEngine::getReplicatorByEntity(const QString &entityName)
{
    const auto it = replicators_.constFind(entityName);
    return it != replicators_.cend() ? std::optional{it.value()}
                                     : std::nullopt;
}

} // namespace Replication
