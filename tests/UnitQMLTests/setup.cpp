#include "setup.h"
#include "loginhelper.h"
#include <QDir>

using namespace Qt::StringLiterals;

namespace
{
const auto screenshotsPath{"screenshots"_L1};
}

Setup::Setup(QObject *parent)
    : QObject{parent}
{
}

void Setup::applicationAvailable() const
{
    LoginHelper::setAppConf();
    QDir{}.mkpath(screenshotsPath);
}

void Setup::init() const
{
}
