#include "Billing/business/pay.h"
#include "Utils/restservice.h"
#include "User/data/localuserstore.h"
#include "Authentication/data/restuserauthentication.h"
#include "Authentication/entity/credential.h"
#include "Authentication/login.h"
#include "Utils/restservice.h"
#include <QTest>
#include <QSignalSpy>
#include <QJsonDocument>
#include <QJsonObject>

class TestPay : public QObject
{
    Q_OBJECT
public:
    explicit TestPay(QObject *parent = nullptr);

private slots:
    void initTestCase();

    void test_shouldPayDebtIfHasDebt() const;
    void test_shouldEmitErrorOccurredIfPaymentIsntPayable();
};



TestPay::TestPay(QObject *parent)
    : QObject{parent}
{

}

void TestPay::initTestCase()
{
    QCoreApplication::setOrganizationDomain("cara.com");
    QCoreApplication::setOrganizationName("Cara");
    QCoreApplication::setApplicationName("CaraTesting");

    Authentication::Business::Login login{};
    QSignalSpy spy{&login, &Authentication::Business::Login::authenticated};

    QVERIFY(spy.isValid());

    Authentication::Data::RestUserAuthentication restAuthenticator{};
    Authentication::Entity::Credential credentials{};
    credentials.setUsername("josevictoriano");
    credentials.setPassword("2077120");
    restAuthenticator.setCredentials(credentials);
    login.setAuthenticator(&restAuthenticator);
    User::Data::LocalUserStore lus{};
    login.setStore(&lus);

    login.authenticate();
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}


void TestPay::test_shouldPayDebtIfHasDebt() const
{
    Utils::RestService restService{};
    Business::Pay pay{};
    pay.setPath("payments");
    restService.append(&pay);

    QSignalSpy spy{&pay, &Business::Pay::paid};

    pay.setManager(std::make_shared<QRestAccessManager>());
    pay.pay({
        {"paymentId", 329},
        {"amount", 20}
    });

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

void TestPay::test_shouldEmitErrorOccurredIfPaymentIsntPayable()
{
    Business::Pay pay{};
    QSignalSpy spy{&pay, &Business::Pay::errorOccurred};
    const QVariantMap map = {
        {"amount", 20}
    };

    pay.setManager(std::make_shared<QRestAccessManager>());
    pay.pay(map);

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestPay)

#include "tst_pay.moc"
