#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Cara/Authentication/business/tokenmanager.h"
#include "Cara/config.h"
#include "Cara/networkrequestsingleton.h"

using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    qputenv("QT_LOGGING_RULES", "qt.qml.binding.removal.info=true"_ba);

    QGuiApplication app(argc, argv);
    app.setOrganizationDomain("cara.com"_L1);
    app.setOrganizationName("Cara"_L1);
    app.setApplicationName("Cara"_L1);
    app.setApplicationVersion(getCaraVersion().toString());

    QQmlApplicationEngine engine;
    const auto tokenManager{engine.singletonInstance<Authentication::Business::TokenManager *>("Cara.Authentication"_L1, "TokenManager"_L1)};
    tokenManager->setToken(&Authentication::NetworkRequestSingleton::instance());

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Cara"_L1, "Main"_L1);

    return app.exec();
}
