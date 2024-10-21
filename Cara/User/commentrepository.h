#ifndef COMMENTREPOSITORY_H
#define COMMENTREPOSITORY_H

#include "comment.h"
#include <QQmlEngine>
#include<QJsonObject>

namespace User
{

class CommentRepository : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    enum class Error
    {
        InvalidCommentEntity,
        FailedToFetch,
        FailedToAdd
    };

    enum class Status
    {
        Idle,
        Busy,
        Error
    };

    using OrderId = int;
    using QObject::QObject;

    virtual ~CommentRepository() = default;

    virtual void fetchCommentsByOrderId(OrderId orderId) = 0;
    virtual void addCommentToOrder(OrderId orderId, const Comment &c) = 0;

signals:
    void commentsFetched(const QJsonArray &comments);
    void commentFetched(const QJsonObject &comment);
    void commentAdded(const QJsonObject &comment);
    void errorOccurred(Error error, const QString &errorString);
};

}

#endif // COMMENTREPOSITORY_H
