#ifndef ORDERCOMMENTREPOSITORY_H
#define ORDERCOMMENTREPOSITORY_H

#include "commentrepository.h"
#include <QNetworkAccessManager>
#include <QProperty>
#include <QQmlEngine>
#include <QRestReply>

class QRestAccessManager;
class QNetworkRequestFactory;
namespace User
{

class OrderCommentRepository : public CommentRepository
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Error error READ error WRITE setError BINDABLE bindableError FINAL)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString BINDABLE bindableErrorString FINAL)
    Q_PROPERTY(Status status READ default BINDABLE bindableStatus)
public:
    Q_ENUM(Status);

    OrderCommentRepository(QObject *parent = nullptr);

    // CommentRepository interface
    void fetchCommentsByOrderId(OrderId orderId) override;
    Q_INVOKABLE void addCommentToOrder(OrderId orderId, const Comment &c) override;

    Error error() const;
    void setError(Error newError);
    QBindable<Error> bindableError();

    QString errorString() const;
    void setErrorString(const QString &newErrorString);
    QBindable<QString> bindableErrorString();

    QBindable<Status> bindableStatus();

private:
    void commentsByOrderIdFetched(QRestReply &reply);
    void commentToOrderAdded(QRestReply &reply);
    void checkReplyError(QRestReply &reply);

private:
    QNetworkAccessManager qnam_;
    QRestAccessManager *manager_;
    std::shared_ptr<QNetworkRequestFactory> api_;
    QProperty<Error> error_;
    QProperty<QString> errorString_;
    QProperty<Status> status_;
};

} // namespace User

#endif // ORDERCOMMENTREPOSITORY_H
