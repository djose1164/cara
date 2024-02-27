#include "androidshareplataform.h"
#include <QJniObject>
#include <QCoreApplication>

namespace
{
constexpr auto className{"com/cara/ShareUtils"};
}

namespace Utils
{
AndroidSharePlataform::AndroidSharePlataform() {}

void AndroidSharePlataform::share(const QUrl &path) const
{
    if (!QJniObject::isClassAvailable(className))
    {
        qCritical() << "ShareUtils::share: " << className << " is not available.";
        return;
    }

    QJniObject activity{QNativeInterface::QAndroidApplication::context()};
    const auto localFile{path.toLocalFile()};
    QJniObject::callStaticMethod<void>(
        className,
        "share",
        "(Ljava/lang/String;Landroid/content/Context;)V",
        QJniObject::fromString(localFile).object<jstring>(),
        activity
    );
}
}
