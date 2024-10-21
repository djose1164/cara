#include "ordercommentrepository.h"
#include "Network/networkrequestsingleton.h"
#include <QRestAccessManager>
#include <QNetworkRequestFactory>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define checkReplySuccess(reply)                                               \
    do {                                                                       \
        if (!(reply).isSuccess())                                              \
        {                                                                      \
            setError(Error::FailedToFetch);                                    \
            setErrorString((reply).errorString());                             \
            status_ = Status::Error;                                           \
            emit errorOccurred(error(), errorString());                        \
            qWarning() << __FUNCTION__ << ": " << errorString_;                \
            return;                                                            \
        }                                                                      \
    } while (0)

using namespace Qt::StringLiterals;
using namespace std::string_literals;

namespace
{
constexpr auto path{"comments/"_L1};
constexpr auto commentsField{"comments"_L1};
constexpr auto commentField{"comment"_L1};
constexpr auto errorField{"error"_L1};
}

namespace User
{

OrderCommentRepository::OrderCommentRepository(QObject *parent)
    : CommentRepository{parent}
    , qnam_{}
    , manager_{new QRestAccessManager{&qnam_, this}}
    , api_{Network::NetworkRequestSingleton::instance().api()}
    , status_{Status::Idle}
{}

void OrderCommentRepository::fetchCommentsByOrderId(OrderId orderId)
{
    status_ = Status::Busy;
    QUrlQuery query;
    query.addQueryItem("order_id"_L1, QString::number(orderId));

    manager_->get(api_->createRequest(path, query),
                  this, &OrderCommentRepository::commentsByOrderIdFetched);
}

void OrderCommentRepository::addCommentToOrder(OrderId orderId, const Comment &c)
{
    qDebug() << "OrderCommentRepository::addCommentToOrder: posting new comment to order...orderId: " << orderId << "comment: " << c.toJson();
    if (!c.isValid())
    {
        setErrorString("OrderCommentRepository::addCommentToOrder: Invalid comment entity: %1"_L1.arg(c.toString()));
        emit errorOccurred(Error::InvalidCommentEntity, errorString_);
        status_ = Status::Error;
        qWarning() << "OrderCommentRepository::addCommentToOrder: " << errorString_;
        return;
    }

    if (orderId < 1)
    {
        setErrorString("OrderCommentRepository::addCommentToOrder: Invalid orderId: %1"_L1.arg(QString::number(orderId)));
        emit errorOccurred(Error::InvalidCommentEntity, errorString_);
        status_ = Status::Error;
        return;
    }

    status_ = Status::Busy;
    QUrlQuery query;
    query.addQueryItem("order_id"_L1, QString::number(orderId));

    manager_->post(api_->createRequest(path, query),
                   QJsonDocument{c.toJson()},
                   this,
                   &OrderCommentRepository::commentToOrderAdded);

    qDebug() << "OrderCommentRepository::addCommentToOrder: request sent...";
}

void OrderCommentRepository::commentsByOrderIdFetched(QRestReply &reply)
{
    try
    {
        checkReplyError(reply);
        if (const auto json{reply.readJson()};
            json && json->isObject() && json->object().contains(commentsField))
        {
            emit commentsFetched((*json)[commentsField].toArray());
        }
    }
    catch (const std::runtime_error &err)
    {
        qWarning() << "OrderCommentRepository::commentsByOrderIdFetched: " << err.what();
        emit errorOccurred(Error::FailedToFetch, err.what());
    }

    status_ = Status::Idle;
}

void OrderCommentRepository::commentToOrderAdded(QRestReply &reply)
{
    qDebug() << "OrderCommentRepository::commentToOrderAdded: handling reply...";
    try
    {
        checkReplyError(reply);
        if (const auto json{reply.readJson()};
            json && json->isObject() && json->object().contains(commentField))
        {
            qDebug() << "OrderCommentRepository::commentToOrderAdded: " << (*json)[commentField].toObject();
            emit commentAdded((*json)[commentField].toObject());
        }
    }
    catch (const std::runtime_error &err)
    {
        emit errorOccurred(Error::FailedToAdd, err.what());
    }

    status_ = Status::Idle;
}

QString OrderCommentRepository::errorString() const
{
    return errorString_;
}

void OrderCommentRepository::setErrorString(const QString &newErrorString)
{
    errorString_ = newErrorString;
}

QBindable<QString> OrderCommentRepository::bindableErrorString()
{
    return &errorString_;
}

QBindable<CommentRepository::Status> OrderCommentRepository::bindableStatus()
{
    return &status_;
}

void OrderCommentRepository::checkReplyError(QRestReply &reply)
{
    if (reply.isSuccess())
        return;

    if (!reply.errorString().isEmpty())
        throw std::runtime_error{reply.errorString().toStdString()};

    if (const auto json{reply.readJson()};
        json and json->isObject() and json->object().contains(errorField))
    {
        const auto err{(*json)[errorField].toString()};
        throw std::runtime_error{err.toStdString()};
    }

    throw std::runtime_error{"OrderCommentRepository::checkReplyError: Error desconocido"s};
}

CommentRepository::Error OrderCommentRepository::error() const
{
    return error_;
}

void OrderCommentRepository::setError(Error newError)
{
    error_ = newError;
}

QBindable<CommentRepository::Error> OrderCommentRepository::bindableError()
{
    return &error_;
}

} // namespace User
