#include "constants.h"
#include "restconfig.h"
#include "apienvironment.h"

using namespace Qt::StringLiterals;

namespace DataLayer {

QUrl Constants::getBase()
{
    QSettings settings;
    return settings.value("base"_L1).toUrl();
}

Constants::Constants(QObject *parent)
    : QObject{parent}
    , testingBase_{QStringLiteral(LOCAL_API_URL)}
    , remoteTesting_{"https://cara-api-testing.onrender.com"_L1}
    , production_{QStringLiteral(REMOTE_API_URL)}
    , base_{Api::getApiPath().toString()}
    , appWidth_{400}
    , appHeight_{640}
{
    QSettings settings;
    settings.setValue("base"_L1, base_);
    setUploadsPath();

    qDebug() << "Base URL: " << base_;
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

void Constants::setUploadsPath()
{
    QSettings settings;
    QString path{settings.value("base"_L1).toString()};

    uploadsPath_ = path.replace("/api"_L1, "/uploads"_L1) + "/"_L1;
}

QString Constants::uploadsPath() const
{
    return uploadsPath_;
}

}
