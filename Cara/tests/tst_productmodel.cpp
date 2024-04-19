#include "Authentication/data/restuserauthentication.h"
#include "Authentication/login.h"
#include "Authentication/entity/credential.h"
#include "Inventory/inventoryrepository.h"
#include "User/data/localuserstore.h"
#include "Inventory/model/productmodel.h"
#include <QTest>
#include <QSignalSpy>
#include <QJsonDocument>
#include <QJsonObject>

class TestProductModel : public QObject
{
    Q_OBJECT
public:
    explicit TestProductModel(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void init();

    void test_shouldEmitUpdatedAfterUpdating() const;
    void test_shouldPopulateModelIfCatalogIsNotEmpty() const;
};



TestProductModel::TestProductModel(QObject *parent)
    : QObject{parent}
{

}

void TestProductModel::initTestCase()
{
    QCoreApplication::setOrganizationDomain("cara.com");
    QCoreApplication::setOrganizationName("Cara");
    QCoreApplication::setApplicationName("CaraTesting");

    Authentication::Business::Login login{};
    Authentication::Entity::Credential c{};
    c.setUsername("cara");
    c.setPassword("1234");
    Authentication::Data::RestUserAuthentication auth{};
    auth.setCredentials(c);
    login.setAuthenticator(&auth);

    User::Data::LocalUserStore userStore{};
    QSignalSpy spy{&login, SIGNAL(authenticated(const QByteArray &))};
    login.setStore(&userStore);

    login.authenticate();
    QVERIFY(spy.wait());
}

void TestProductModel::init()
{

}

void TestProductModel::test_shouldEmitUpdatedAfterUpdating() const
{
    Model::ProductModel pm{};
    Data::InventoryRepository repository{};
    pm.setRepository(&repository);

    QSignalSpy spy{&pm, &Model::ProductModel::updated};

    pm.update(1);
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

void TestProductModel::test_shouldPopulateModelIfCatalogIsNotEmpty() const
{
    Model::ProductModel pm{};
    Data::InventoryRepository repository{};
    pm.setRepository(&repository);

    QSignalSpy spy{&repository, &Data::InventoryRepository::fetchedAll};

    pm.update(1);
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
    QVERIFY(pm.count() > 0);
}

QTEST_MAIN(TestProductModel)

#include "tst_productmodel.moc"
