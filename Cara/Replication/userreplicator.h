#ifndef USERREPLICATOR_H
#define USERREPLICATOR_H

#include "entityreplicator.h"

namespace Replication
{

class UserReplicator : public Replication::EntityReplicator
{
    Q_OBJECT
public:
    explicit UserReplicator(QObject *parent = nullptr);

    void apply(const QJsonObject &payload) override;
    QString entityName() const override;

private:
    bool isPayloadValid(const QJsonObject &payload);
};

} // namespace Replication

#endif // USERREPLICATOR_H
