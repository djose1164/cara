#include "Inventory/inventoryrepository.h"
#include "Authentication/data/restuserauthentication.h"
#include "Authentication/entity/credential.h"
#include "Authentication/login.h"
#include "User/data/localuserstore.h"
#include <QTest>
#include <QSignalSpy>
#include <QJsonDocument>
#include <QJsonObject>

class TestInventoryRepository : public QObject
{
    Q_OBJECT
public:
    explicit TestInventoryRepository(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void init();

    void test_shouldEmitFetchedAllUponFetchingAll() const;
};



TestInventoryRepository::TestInventoryRepository(QObject *parent)
    : QObject{parent}
{

}

void TestInventoryRepository::initTestCase()
{
    QCoreApplication::setOrganizationDomain("cara.com");
    QCoreApplication::setOrganizationName("Cara");
    QCoreApplication::setApplicationName("CaraTesting");

    Authentication::Business::Login login{};
    Authentication::Entity::Credential c{};
    c.setUsername("josevictoriano");
    c.setPassword("2077120");
    Authentication::Data::RestUserAuthentication auth{};
    auth.setCredentials(c);
    login.setAuthenticator(&auth);

    User::Data::LocalUserStore userStore{};
    login.setStore(&userStore);

    QSignalSpy spy{&login, SIGNAL(authenticated(const QByteArray &))};
    QVERIFY(spy.isValid());

    login.authenticate();
    QVERIFY(spy.wait());
}

void TestInventoryRepository::init()
{

}

void TestInventoryRepository::test_shouldEmitFetchedAllUponFetchingAll() const
{
    Data::InventoryRepository repository{};

    QSignalSpy spy{&repository, &Data::InventoryRepository::fetchedAll};

    repository.fetchAll(1);
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestInventoryRepository)

#include "tst_inventoryrepository.moc"
