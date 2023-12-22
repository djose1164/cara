#ifndef USERREPOSITORYIMPL_H
#define USERREPOSITORYIMPL_H

#include "userrepository.h"
#include <QNetworkAccessManager>

namespace Data
{
class UserRepositoryImpl final : public UserRepository
{
    Q_OBJECT
public:
    explicit UserRepositoryImpl(QObject *parent = nullptr);

    // UserRepository interface
    void read(int id) override;

    void update(const Entity::UserEntity &user) override;

private:
    QNetworkRequest createRequest(const QString &endpoint);

private:
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
};
}

#endif // USERREPOSITORYIMPL_H
