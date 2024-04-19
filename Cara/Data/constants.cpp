#include "constants.h"
#include "restconfig.h"

using namespace Qt::StringLiterals;

namespace DataLayer {
QUrl Constants::getBase()
{
    QSettings settings;
    return settings.value("base"_L1).toUrl();
}

Constants::Constants()
#ifdef Q_OS_ANDROID
    // : testingBase_{"http://10.0.2.2:5000"_L1}
    : testingBase_{"http://192.168.0.13:5000"_L1}
#else
    : testingBase_{QStringLiteral(LOCAL_API_URL)}
#endif
    , remoteTesting_{"https://cara-api-testing.onrender.com"_L1}
    , production_{QStringLiteral(REMOTE_API_URL)}
    , appWidth_{400}
    , appHeight_{640}
{
#ifdef QT_DEBUG
    base_ = testingBase_ + "/api"_L1;
#else
    base_ = production_ + "/api"_L1;
#endif
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
