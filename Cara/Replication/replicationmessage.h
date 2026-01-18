#ifndef REPLICATIONMESSAGE_H
#define REPLICATIONMESSAGE_H

#include <QJsonObject>

namespace Replication
{

struct ReplicationMessage
{
    QString entity;
    QJsonObject payload;
};

}

#endif // REPLICATIONMESSAGE_H
