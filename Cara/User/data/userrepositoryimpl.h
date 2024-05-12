#ifndef USERREPOSITORYIMPL_H
#define USERREPOSITORYIMPL_H

#include "userrepository.h"
#include <QNetworkAccessManager>
#include <QNetworkRequestFactory>

namespace Data
{
class UserRepositoryImpl final : public UserRepository
{
    Q_OBJECT
public:
    explicit UserRepositoryImpl(QObject *parent = nullptr);

    // UserRepository interface
    void read(int id) override;

    QNetworkReply *readFavorites(UserId id) override;

    void update(const Entity::UserEntity &user) override;

    void updatePassword(UserId userId, const QVariantMap &data) override;

    void deleteUser(UserId userId) override;

    QString path() const;
    void setPath(const QString &newPath);

private:
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
    QString path_;
    const std::shared_ptr<QNetworkRequestFactory> api_;
};
}

#endif // USERREPOSITORYIMPL_H
