#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <qqmlextensionplugin.h>
#include "Cara/ProductInventory/sqlitedatabase.h"

#include "Cara/config.h"

Q_IMPORT_QML_PLUGIN(Cara_ProductInventoryPlugin)
Q_IMPORT_QML_PLUGIN(Cara_ShoppingPlugin)
Q_IMPORT_QML_PLUGIN(Cara_BillingPlugin)

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
    ProductInventory::SQLiteDatabase::initDatabase();

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Cara"_L1, "Main"_L1);

    return app.exec();
}
