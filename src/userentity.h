#ifndef USERENTITY_H
#define USERENTITY_H

#include <QString>

class UserEntity
{
private:
    QString username;
    QString forename;
    QString surname;
    QString email;
    QString password;

public:
    UserEntity();
    bool operator ==(const UserEntity &other);
};

#endif // USERENTITY_H
