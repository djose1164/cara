#include "constants.h"

namespace DataLayer {
QUrl Constants::getBase()
{
    QSettings settings;
    return settings.value("base").toUrl();
}

Constants::Constants()
#ifdef Q_OS_ANDROID
    : testingBase_{"http://10.0.2.2:5000"}
#else
    : testingBase_{"http://127.0.0.1:5000"}
#endif
    , remoteTesting_{"https://cara-api-testing.onrender.com"}
    , production_{"https://cara-api.onrender.com"}
    , appWidth_{400}
    , appHeight_{640}
{
#ifdef QT_DEBUG
    base_ = testingBase_ + "/api";
#else
    base_ = production_ + "/api";
#endif
    QSettings settings;
    settings.setValue("base", base_);
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
