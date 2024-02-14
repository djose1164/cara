#include "Authentication/data/restuserauthentication.h"
#include "Authentication/login.h"
#include "Salesperson/salespersoncontroller.h"
#include <QTest>
#include <QSignalSpy>
#include <QJsonDocument>
#include <QJsonObject>
#include "data/localuserstore.h"

class TestSalespersonController : public QObject
{
    Q_OBJECT
public:
    explicit TestSalespersonController(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void test_shouldGetSalespersonById() const;
    void test_shouldGetSalespeopleByAdminId() const;
};



TestSalespersonController::TestSalespersonController(QObject *parent)
    : QObject{parent}
{

}

void TestSalespersonController::initTestCase()
{
    QCoreApplication::setOrganizationDomain("cara.com");
    QCoreApplication::setOrganizationName("Cara");
    QCoreApplication::setApplicationName("CaraTesting");

    Authentication::Business::Login login{};
    QSignalSpy spy{&login, &Authentication::Business::Login::authenticated};

    QVERIFY(spy.isValid());

    Authentication::Data::RestUserAuthentication restAuthenticator{};
    Authentication::Entity::Credential credentials{};
    credentials.setUsername("cara");
    credentials.setPassword("1234");
    restAuthenticator.setCredentials(credentials);
    login.setAuthenticator(&restAuthenticator);
    User::Data::LocalUserStore lus{};
    login.setStore(&lus);

    login.authenticate();
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

void TestSalespersonController::test_shouldGetSalespersonById() const
{
    Controller::SalespersonController spController{};
    spController.setSalespersonId(1);
    QSignalSpy spy{&spController, SIGNAL(fetched(void))};

    QVERIFY(spy.isValid());

    spController.readSalesperson();

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
    QVERIFY(spController.buffer().isEmpty() == false);

    const auto obj{
        QJsonDocument::fromJson(spController.buffer())
            .object()
    };

    QVERIFY(obj.contains("salesperson"));

    const auto doc{QJsonDocument(obj["salesperson"].toObject())};
    const auto sp{Entity::Salesperson::fromJson(doc.toJson())};

    QVERIFY(sp.isValid());
}

void TestSalespersonController::test_shouldGetSalespeopleByAdminId() const
{
    Controller::SalespersonController spController{};
    spController.setAdminId(5);
    QSignalSpy spy{&spController, SIGNAL(fetched(void))};

    QVERIFY(spy.isValid());

    spController.readSalespeople();

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
    QVERIFY(spController.buffer().isEmpty() == false);

    const auto obj{
        QJsonDocument::fromJson(spController.buffer())
            .object()
    };

    QVERIFY(obj.contains("salespersons"));

    const bool result{obj["salespersons"].isArray()};

    QVERIFY(result);
}

QTEST_MAIN(TestSalespersonController)

#include "tst_salespersoncontroller.moc"
