#include "inventoryrepository.h"
#include "localuserstore.h"
#include "login.h"
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

    Business::Login login{};
    login.setUsername("josevictoriano");
    login.setPassword("2077120");

    Business::LocalUserStore userStore{};
    QSignalSpy spy{&login, SIGNAL(authenticated(void))};
    login.setUserStore(&userStore);

    login.authenticateCredentials();
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
