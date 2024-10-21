#include "comment.h"

#include <QJsonDocument>
#include <QJsonObject>

#define checkNotContains(json, field)                                              \
    do {                                                                           \
        if (!(json).contains((field)))                                             \
        {                                                                          \
          throw std::invalid_argument{errorMsg.arg((field)).toUtf8()};             \
        }                                                                          \
    } while (0)

using namespace Qt::StringLiterals;

namespace
{
constexpr auto idField{"id"_L1};
constexpr auto userIdField{"user_id"_L1};
constexpr auto contentField{"content"_L1};
constexpr auto createdAtField{"created_at"_L1};
constexpr auto updatedAtField{"updated_at"_L1};
constexpr auto userField{"user"_L1};
constexpr auto errorMsg{"QJsonObject doesn't contains %1."_L1};
}

namespace User
{

Comment Comment::fromJson(const QJsonObject &json)
{
    try
    {
        validateJson(json);

        Comment c;
        c.setCommentId(json[idField].toInt());
        c.setContent(json[contentField].toString());

        c.setUserId(json[userIdField].toInt());
        c.setUser(Entity::UserEntity::fromJson(QJsonDocument{json}.toJson()));

        c.setCreatedAt(QDateTime::fromString(json[createdAtField].toString(), Qt::ISODate));
        c.createdAt_.setTimeZone(QTimeZone{QTimeZone::UTC});

        return c;
    }
    catch (const std::invalid_argument &err)
    {
        qWarning() << "Comment::fromJson: " << err.what();
    }
    return Comment{};
}

Comment Comment::fromMap(const QVariantMap &map)
{
    Comment c{};
    c.setContent(map[contentField].toString());
    c.setUserId(map["userId"_L1].toInt());
    return c;
}

void Comment::validateJson(const QJsonObject &json)
{
    if (json.isEmpty())
    {
        qWarning() << "Comment::validateJson: " << json;
        throw std::invalid_argument{"QJsonObject is empty."};
    }
    checkNotContains(json, idField);
    checkNotContains(json, userIdField);
    checkNotContains(json, contentField);
    checkNotContains(json, createdAtField);
    checkNotContains(json, updatedAtField);
    // if (not json.contains(idField))
    //     throw std::invalid_argument{errorMsg.arg(idField).toUtf8()};
    // if (not json.contains(userIdField))
    //     throw std::invalid_argument{errorMsg.arg(userIdField).toUtf8()};
    // if (not json.contains(contentField))
    //     throw std::invalid_argument{errorMsg.arg(contentField).toUtf8()};
    // if (not json.contains(createdAtField))
    //     throw std::invalid_argument{errorMsg.arg(createdAtField).toUtf8()};
    // if (not json.contains(updatedAtField))
    //     throw std::invalid_argument{errorMsg.arg(updatedAtField).toUtf8()};
}

Comment::Comment()
    : commentId_{}
    , userId_{}
    , content_{}
    , createdAt_{}
    , updatedAt_{}
{}

int Comment::commentId() const
{
    return commentId_;
}

void Comment::setCommentId(int newCommentId)
{
    commentId_ = newCommentId;
}

int Comment::userId() const
{
    return userId_;
}

void Comment::setUserId(int newUserId)
{
    userId_ = newUserId;
}

QString Comment::content() const
{
    return content_;
}

void Comment::setContent(const QString &newContent)
{
    content_ = newContent;
}

QDateTime Comment::createdAt() const
{
    return createdAt_;
}

void Comment::setCreatedAt(const QDateTime &newCreatedAt)
{
    createdAt_ = newCreatedAt;
}

QDateTime Comment::updatedAt() const
{
    return updatedAt_;
}

void Comment::setUpdatedAt(const QDateTime &newUpdatedAt)
{
    updatedAt_ = newUpdatedAt;
}

bool Comment::isValid() const
{
    return userId_ > 0 and not content_.isEmpty();
}

QJsonObject Comment::toJson() const
{
    QJsonObject obj;
    obj[idField] = commentId_;
    obj[userIdField] = userId_;
    obj[contentField] = content_;
    obj[createdAtField] = createdAt_.toString(Qt::ISODate);
    obj[updatedAtField] = updatedAt_.toString(Qt::ISODate);
    return obj;
}

Entity::UserEntity Comment::user() const
{
    return user_;
}

void Comment::setUser(const Entity::UserEntity &newUser)
{
    user_ = newUser;
}

QString Comment::toString() const
{
    return QJsonDocument{toJson()}.toJson();
}

} // namespace User
