#include "constants.h"

namespace DataLayer {
Constants::Constants()
    : localBase_{"http://127.0.0.1:5000"}
    , remoteBase_{"https://cara-rd.live"}
    , appWidth_{400}
    , appHeight_{640}
{
#ifdef QT_DEBUG
    base_ = localBase_ + "/api";
#else
    base_ = remoteBase_ + "/api";
#endif
}

void Constants::setBase(const QString &base)
{
    if (base_ != base)
    {
        base_ = base;
        emit baseChanged();
    }
}

QString Constants::base() const
{
    return base_;
}

int Constants::appWidth() const
{
    return appWidth_;
}

int Constants::appHeight() const
{
    return appHeight_;
}
}
