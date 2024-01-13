#include "salesperson/data/salespersonmanager.h"
#include "user/localuserstore.h"
#include "authentication/login.h"
#include "salesperson/salesperson.h"
#include <QTest>
#include <QSignalSpy>
#include <QJsonDocument>
#include <QJsonObject>

class TestSalespersonManager : public QObject
{
    Q_OBJECT
public:
    explicit TestSalespersonManager(QObject *parent = nullptr);

private slots:
    void initTestCase();

    void test_shouldReadSalespersonById() const;
    void test_shouldReturnIfDataIsValidUpondReadingSalesperson() const;
};



TestSalespersonManager::TestSalespersonManager(QObject *parent)
    : QObject{parent}
{

}

void TestSalespersonManager::initTestCase()
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

void TestSalespersonManager::test_shouldReadSalespersonById() const
{
    Data::SalespersonManager spManager{};
    QSignalSpy spy{&spManager, SIGNAL(fetched(void))};

    spManager.read(1);

    QVERIFY(spy.wait());
    QCOMPARE(spy.count(), 1);

    const auto obj{QJsonDocument::fromJson(spManager.buffer()).object()};

    QVERIFY(obj.contains("salesperson"));

    const auto doc{QJsonDocument(obj["salesperson"].toObject())};
    const auto sp{Entity::Salesperson::fromJson(doc.toJson())};

    QVERIFY2(sp.isValid(), "salesperson entity is malformed.");
}

void TestSalespersonManager::test_shouldReturnIfDataIsValidUpondReadingSalesperson() const
{
    Data::SalespersonManager spManager{};
    QSignalSpy spy{&spManager, SIGNAL(fetched(void))};

    spManager.read(1);

    QVERIFY(spy.wait());
    QCOMPARE(spy.count(), 1);

    const auto obj{QJsonDocument::fromJson(spManager.buffer()).object()};

    QVERIFY2(obj.contains("salesperson"), "response doesn't contain salesperson object.");

    const auto sp{obj["salesperson"].toObject()};

    QVERIFY2(sp.contains("id"), "response doesn't contain salesperson's id.");
    QVERIFY2(sp.contains("buy_orders"), "response doesn't contain salesperson's buy_orders.");
    QVERIFY2(sp.contains("credit_available"), "response doesn't contain salesperson's credit_available.");
    QVERIFY2(sp.contains("credit_consumed"), "response doesn't contain salesperson's credit_consumed.");
    QVERIFY2(sp.contains("credit_limit"), "response doesn't contain salesperson's credit_limit.");
    QVERIFY2(sp.contains("customers"), "response doesn't contain salesperson's customers.");
    QVERIFY2(sp.contains("inventory"), "response doesn't contain salesperson's inventory.");
    QVERIFY2(sp.contains("salesperson_credit"), "response doesn't contain salesperson's salesperson_credit.");
    QVERIFY2(sp.contains("salesperson_type"), "response doesn't contain salesperson's salesperson_type.");
    QVERIFY2(sp.contains("salesperson_type_id"), "response doesn't contain salesperson's salesperson_type_id.");
    QVERIFY2(sp.contains("user"), "response doesn't contain salesperson's user.");
    QVERIFY2(sp.contains("warehouse"), "response doesn't contain salesperson's warehouse.");
}

QTEST_MAIN(TestSalespersonManager)

#include "tst_salespersonmanager.moc"
