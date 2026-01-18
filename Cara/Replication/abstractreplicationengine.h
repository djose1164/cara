#ifndef ABSTRACTREPLICATIONENGINE_H
#define ABSTRACTREPLICATIONENGINE_H

#include "replicationmessage.h"
#include <QObject>

namespace Replication
{

class EntityReplicator;

using ReplicationMessageList = QList<ReplicationMessage>;

class AbstractReplicationEngine : public QObject
{
    Q_OBJECT
public:
    using ReplicationPlan = QVariantMap;

    using QObject::QObject;
    virtual ~AbstractReplicationEngine() = default;

    virtual void run(const ReplicationMessageList &plan) = 0;
    virtual void registerReplicator(EntityReplicator *r) = 0;
    virtual std::optional<EntityReplicator *> getReplicatorByEntity(const QString &entityName) = 0;

signals:
    void finished();
    void errorOccurred(const QString &errorString);
};

}

#endif // ABSTRACTREPLICATIONENGINE_H
