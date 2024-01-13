#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "userentity.h"
#include <QObject>

namespace Data
{
class UserRepository : public QObject
{
    Q_OBJECT
public:
    explicit UserRepository(QObject *parent = nullptr);

    virtual ~UserRepository();

    virtual void read(int id) = 0;

    virtual void update(const Entity::UserEntity &user) = 0;

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

private:
    QString base_;
    QByteArray buffer_;
    QString errorString_;
};
}

#endif // USERREPOSITORY_H
