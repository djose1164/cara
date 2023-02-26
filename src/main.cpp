#include "rest.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationDomain("cara.com");
    app.setOrganizationName("Cara");
    app.setApplicationName("Cara");
    app.setApplicationVersion("0.1.3");
    qputenv("QT_NO_ANDROID_TEXT_HANDLES", "1");

    QQuickStyle::setStyle("Material");
    qmlRegisterType<Rest>("com.cara", 1, 0, "Rest");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/main/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
