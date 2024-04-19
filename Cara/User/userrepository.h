#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "userentity.h"
#include <QObject>

class QNetworkReply;

namespace Data
{
class UserRepository : public QObject
{
    Q_OBJECT
public:
    using UserId = int;

    explicit UserRepository(QObject *parent = nullptr);

    virtual ~UserRepository();

    virtual void read(int id) = 0;

    virtual QNetworkReply *readFavorites(UserId id) = 0;

    virtual void update(const Entity::UserEntity &user) = 0;

    virtual void updatePassword(UserId userId, const QVariantMap &data) = 0;

    QString base() const;
    void setBase(const QString &newBase);

    QByteArray buffer() const;
    void setBuffer(const QByteArray &newBuffer);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

signals:
    void fetched();
    void updated();
    void errorOccurred();
    void passwordUpdated();

private:
    QString base_;
    QByteArray buffer_;
    QString errorString_;
};
}

#endif // USERREPOSITORY_H
