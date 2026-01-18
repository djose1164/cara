#ifndef ENTITYREPLICATOR_H
#define ENTITYREPLICATOR_H

#include <QObject>

namespace Replication
{

class EntityReplicator : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    virtual ~EntityReplicator() = default;

    virtual QString entityName() const = 0;
    virtual void apply(const QJsonObject &payload) = 0;

signals:
    void finished();
    void errorOccurred(const QString &errorString);
};

} // namespace Replication

#endif // ENTITYREPLICATOR_H
