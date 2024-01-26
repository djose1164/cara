#include "business/pay.h"
#include "localuserstore.h"
#include "login.h"
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
    void init();

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

    Business::Login login{};
    login.setUsername("josevictoriano");
    login.setPassword("2077120");

    Business::LocalUserStore userStore{};
    QSignalSpy spy{&login, SIGNAL(authenticated(void))};
    login.setUserStore(&userStore);

    login.authenticateCredentials();
    QVERIFY(spy.wait());
}

void TestPay::init()
{

}

void TestPay::test_shouldPayDebtIfHasDebt() const
{
    Business::Pay pay{};
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
