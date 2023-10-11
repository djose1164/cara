#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    qputenv("QT_LOGGING_RULES", "qt.qml.binding.removal.info=true");

    QGuiApplication app(argc, argv);
    app.setOrganizationDomain("cara.com");
    app.setOrganizationName("Cara");
    app.setApplicationName("Cara");
    app.setApplicationVersion("0.13.0");

    QQmlApplicationEngine engine;
    QQuickStyle::setStyle("Material");
    const QUrl url(u"qrc:/main/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
