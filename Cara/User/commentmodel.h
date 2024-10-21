#ifndef COMMENTMODEL_H
#define COMMENTMODEL_H

#include "commentrepository.h"

#include <QAbstractListModel>
#include <QProperty>
#include <QQmlEngine>
#include <QJsonArray>

#define STATUS_SETTER(x)                                                       \
  void set##x() noexcept { status_ = Status::x; }

namespace User
{

class CommentModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(CommentRepository *repository READ repository WRITE setRepository BINDABLE bindableRepository FINAL)
    Q_PROPERTY(Status status READ default BINDABLE bindableStatus FINAL)
    Q_PROPERTY(QString errorString READ default BINDABLE bindableErrorString FINAL)
    QML_ELEMENT
public:
    enum class RoleNames
    {
        CommentIdRole,
        CreatedAtRole,
        UpdatedAtRole,
        UserIdRole,
        AuthorRole,
        ContentRole
    };

    enum class Status
    {
        Idle,
        Error, // Repository reported an error.
        Busy // Performing an async operation
    };
    Q_ENUM(Status)

    using OrderId = int;

    explicit CommentModel(QObject *parent = nullptr);

    CommentRepository *repository() const;
    void setRepository(CommentRepository *newRepository);
    QBindable<CommentRepository> bindableRepository();

    QBindable<Status> bindableStatus();

    QBindable<QString> bindableErrorString();

    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QHash<int, QByteArray> roleNames() const override;

public slots:
    void fetchCommentsByOrderId(OrderId orderId) noexcept;
    void addComment(const QVariantMap &comment) noexcept;

private slots:
    void set(const QJsonArray &commentsArray);
    void checkAndThrowIfMissing(const QVariantMap &comment);

private:
    void append(auto &&comment);
    STATUS_SETTER(Busy)
    STATUS_SETTER(Error)
    STATUS_SETTER(Idle)

signals:
    void commentAdded();
    void errorOccurred(const QString &errorString);

private:
    QProperty<CommentRepository *> repository_;
    QList<Comment> data_;
    QProperty<Status> status_;
    QProperty<QString> errorString_;
};

} // namespace User

#undef STATUS_SETTER

#endif // COMMENTMODEL_H
