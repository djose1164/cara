#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    qputenv("QT_LOGGING_RULES", "qt.qml.binding.removal.info=true");

    QApplication app(argc, argv);
    app.setOrganizationDomain("cara.com");
    app.setOrganizationName("Cara");
    app.setApplicationName("Cara");
    app.setApplicationVersion("0.16.0");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qt/qml/Cara/main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
