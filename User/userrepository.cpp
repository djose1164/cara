#include "userrepository.h"
#include <QSettings>

namespace Data
{
UserRepository::UserRepository(QObject *parent)
    : QObject{parent}
    , buffer_{}
    , errorString_{}
{
    QSettings settings;
    base_ = settings.value("base").toString() + "/users";
}

UserRepository::~UserRepository()
{

}

QString UserRepository::base() const
{
    return base_;
}

void UserRepository::setBase(const QString &newBase)
{
    base_ = newBase;
}

QByteArray UserRepository::buffer() const
{
    return buffer_;
}

void UserRepository::setBuffer(const QByteArray &newBuffer)
{
    buffer_ = newBuffer;
}

QString UserRepository::errorString() const
{
    return errorString_;
}

void UserRepository::setErrorString(const QString &newErrorString)
{
    errorString_ = newErrorString;
}
}
