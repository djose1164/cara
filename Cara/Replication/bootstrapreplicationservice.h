#ifndef BOOTSTRAPREPLICATIONSERVICE_H
#define BOOTSTRAPREPLICATIONSERVICE_H

#include <QQmlEngine>

namespace Replication
{

class AbstractReplicationEngine;

class BootstrapReplicationService : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    using UserId = int;

    explicit BootstrapReplicationService(QObject *parent = nullptr);

public slots:
    void replicateForUser(const QVariantMap &user);
    void replicateGlobalData();

signals:
    void finished();
    void errorOccurred(const QString errorString);

private:
    AbstractReplicationEngine *engine_;
};

} // namespace Replication

#endif // BOOTSTRAPREPLICATIONSERVICE_H
