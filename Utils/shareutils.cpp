#include "shareutils.h"
#ifdef Q_OS_ANDROID
#include "androidshareplataform.h"
#endif

namespace
{
constexpr auto className{"com/cara/ShareUtils"};
}
namespace Utils
{
ShareUtils::ShareUtils(QObject *parent)
    : QObject{parent}
#ifdef Q_OS_ANDROID
    , sharePlataform_{std::make_unique<AndroidSharePlataform>()}
#else
    , sharePlataform_{}
#endif
{}

void ShareUtils::share(const QUrl &path)
{
    if (sharePlataform_)
        sharePlataform_->share(path);
}
}
