#include "commentmodel.h"
#include "validationerror.h"

using namespace Qt::StringLiterals;

namespace
{
constexpr auto orderIdField{"order_id"_L1};
constexpr auto orderIdKey{"orderId"_L1};
constexpr auto contentField{"content"_L1};
constexpr auto userIdField{"user_id"_L1};
constexpr auto userIdKey{"userId"_L1};
}

namespace User
{

CommentModel::CommentModel(QObject *parent)
    : QAbstractListModel{parent}
    , repository_{}
    , data_{}
    , status_{Status::Idle}
    , errorString_{}
{}

CommentRepository *CommentModel::repository() const
{
    return repository_;
}

void CommentModel::setRepository(CommentRepository *newRepository)
{
    if (repository_ == newRepository)
        return;
    if (repository_)
        repository_->disconnect(this);

    repository_ = newRepository;

    if (repository_)
    {
        connect(repository_, &CommentRepository::commentsFetched, this, &CommentModel::set);
        connect(repository_,
                &CommentRepository::errorOccurred,
                this,
                [this](CommentRepository::Error e, const QString es)
                {
                    setError();
                    errorString_ = es;
                    emit errorOccurred(es);
                });
    }
}

QBindable<CommentRepository> CommentModel::bindableRepository()
{
    return &repository_;
}

QBindable<CommentModel::Status> CommentModel::bindableStatus()
{
    return &status_;
}

QBindable<QString> CommentModel::bindableErrorString()
{
    return &errorString_;
}

int CommentModel::rowCount(const QModelIndex &parent) const
{
    return data_.count();
}

QVariant CommentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const auto &comment{data_.at(index.row())};
    switch (static_cast<RoleNames>(role))
    {
    case RoleNames::CommentIdRole:
        return comment.commentId();
    case RoleNames::CreatedAtRole:
        return comment.createdAt();
    case RoleNames::UpdatedAtRole:
        return comment.updatedAt();
    case RoleNames::UserIdRole:
        return comment.userId();
    case RoleNames::AuthorRole:
        return comment.user().fullname();
    case RoleNames::ContentRole:
        return comment.content();
    }
}

bool CommentModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    auto &comment{data_[index.row()]};
    switch (static_cast<RoleNames>(role))
    {
    case RoleNames::CommentIdRole:
    case RoleNames::CreatedAtRole:
    case RoleNames::UpdatedAtRole:
    case RoleNames::UserIdRole:
    case RoleNames::AuthorRole:
        return false;
    case RoleNames::ContentRole:
        comment.setContent(value.toString());
        emit dataChanged(index, index, {role});
        return true;
    }
}

QHash<int, QByteArray> CommentModel::roleNames() const
{
    using enum RoleNames;
    return {
        {int(CommentIdRole), "commentId"_ba},
        {int(CreatedAtRole), "createdAt"_ba},
        {int(UpdatedAtRole), "updatedAt"_ba},
        {int(UserIdRole), "userId"_ba},
        {int(AuthorRole), "author"_ba},
        {int(ContentRole), "content"_ba},
    };
}

void CommentModel::fetchCommentsByOrderId(OrderId orderId) noexcept
{
    if (!repository_)
    {
        emit errorOccurred("CommentModel::fetchCommentsByOrderId: repository was not set"_L1);
        return;
    }
    if (orderId < 1)
    {
        emit errorOccurred("CommentModel::fetchCommentsByOrderId: invalid orderId: %1"_L1.arg(
            QString::number(orderId)));
        return;
    }

    setBusy();
    repository_->fetchCommentsByOrderId(orderId);
}

void CommentModel::addComment(const QVariantMap &comment) noexcept
{
    try
    {
        checkAndThrowIfMissing(comment);
        const int orderId{comment[orderIdKey].toInt()};
        Q_ASSERT(orderId > 0);
        Comment c{};
        c.setContent(comment[contentField].toString());
        c.setUserId(comment[userIdKey].toInt());

        repository_->addCommentToOrder(orderId, c);

        connect(repository_,
                &CommentRepository::commentAdded,
                this,
                [this](const QJsonObject &json)
                {
                    auto comment{Comment::fromJson(json)};
                    if (!comment.isValid())
                    {
                        emit errorOccurred(
                            "CommentModel::addComment: created comment is not valid: %1"_L1.arg(
                                comment.toString()));
                        return;
                    }
                   append(std::move(comment));
                });
    }
    catch (const ValidationError &err)
    {
        emit errorOccurred(err.what());
    }
}

void CommentModel::set(const QJsonArray &commentsArray)
{
    qDebug() << "CommentModel::set: setting comments...";
    beginResetModel();
    data_.clear();
    for (const auto &jsonValue : commentsArray)
    {
        auto &&newComment{Comment::fromJson(jsonValue.toObject())};
        if (!newComment.isValid())
        {
            setError();
            emit errorOccurred("CommentModel::set: Failed to create comment entity: %1"_L1.arg(
                newComment.toString()));
            qWarning() << "CommentModel::set: Failed to create comment entity."_L1;
            endResetModel();
            return;
        }

        data_.append(newComment);
    }
    setIdle();
    endResetModel();
}

void CommentModel::checkAndThrowIfMissing(const QVariantMap &comment)
{
    constexpr auto msg{"CommentModel::addComment: comment doesn't contains %1."_L1};
    if (!comment.contains(orderIdKey))
        throw ValidationError{msg.arg(orderIdKey)};

    if (!comment.contains(contentField))
        throw ValidationError{msg.arg(contentField)};

    if (!comment.contains(userIdKey))
        throw ValidationError{msg.arg(userIdKey)};
}

void CommentModel::append(auto &&comment)
{
    const auto count{data_.count()};
    beginInsertRows(QModelIndex{}, count, count);
    data_.append(comment);
    endInsertRows();
}

} // namespace User
