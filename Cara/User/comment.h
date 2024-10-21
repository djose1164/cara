#ifndef COMMENT_H
#define COMMENT_H

#include <QQmlEngine>
#include "userentity.h"

namespace User
{

class Comment
{
    Q_GADGET
    Q_PROPERTY(int commentId READ commentId WRITE setCommentId FINAL)
    Q_PROPERTY(int userId READ userId WRITE setUserId FINAL)
    Q_PROPERTY(QString content READ content WRITE setContent FINAL)
    Q_PROPERTY(QDateTime createdAt READ createdAt WRITE setCreatedAt FINAL)
    Q_PROPERTY(QDateTime updatedAt READ updatedAt WRITE setUpdatedAt FINAL)
    Q_PROPERTY(bool isValid READ isValid FINAL)
    Q_PROPERTY(Entity::UserEntity user READ user WRITE setUser FINAL)
    QML_VALUE_TYPE(comment)
public:
    static Comment fromJson(const QJsonObject &json);
    static Comment fromMap(const QVariantMap &map);
    static void validateJson(const QJsonObject &json);

    Comment();

    int commentId() const;
    void setCommentId(int newCommentId);

    int userId() const;
    void setUserId(int newUserId);

    QString content() const;
    void setContent(const QString &newContent);

    QDateTime createdAt() const;
    void setCreatedAt(const QDateTime &newCreatedAt);

    QDateTime updatedAt() const;
    void setUpdatedAt(const QDateTime &newUpdatedAt);

    bool isValid() const;

    QJsonObject toJson() const;

    Entity::UserEntity user() const;
    void setUser(const Entity::UserEntity &newUser);

    QString toString() const;

private:
    int commentId_;
    int userId_;
    Entity::UserEntity user_;
    QString content_;
    QDateTime createdAt_;
    QDateTime updatedAt_;
};

} // namespace User

#endif // COMMENT_H
