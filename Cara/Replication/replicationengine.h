#ifndef REPLICATIONENGINE_H
#define REPLICATIONENGINE_H

#include "abstractreplicationengine.h"
#include <QMap>
#include <QObject>

namespace Replication
{

class EntityReplicator;

class ReplicationEngine : public AbstractReplicationEngine
{
    Q_OBJECT
public:
    explicit ReplicationEngine(QObject *parent = nullptr);
    ~ReplicationEngine() override;

    void run(const ReplicationMessageList &plan) override;
    void registerReplicator(EntityReplicator *r) override;
    std::optional<EntityReplicator *> getReplicatorByEntity(const QString &entityName) override;

private:
    QMap<QString, EntityReplicator *> replicators_;
};

} // namespace Replication

#endif // REPLICATIONENGINE_H
