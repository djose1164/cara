#include "Billing/business/pay.h"
#include "restservice.h"
#include "loginhelper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>
#include <QRestAccessManager>.h>

class TestPay : public QObject
{
    Q_OBJECT
public:
    explicit TestPay(QObject *parent = nullptr);

private slots:
    void initTestCase();

    void test_shouldPayDebtIfHasDebt() const;
    void test_shouldEmitErrorOccurredIfPaymentIsntPayable();

private:
    std::unique_ptr<QNetworkAccessManager> qnam_;
};



TestPay::TestPay(QObject *parent)
    : QObject{parent}
{

}

void TestPay::initTestCase()
{
    LoginHelper::login();
    qnam_ = std::make_unique<QNetworkAccessManager>();
}


void TestPay::test_shouldPayDebtIfHasDebt() const
{
    Cara::RestService restService{};
    Business::Pay pay{};
    pay.setPath("payments");
    restService.append(&pay);

    QSignalSpy spy{&pay, &Business::Pay::paid};

    auto &&a{&qnam_};
    pay.setManager(std::make_shared<QRestAccessManager>(qnam_.get()));
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

    pay.setManager(std::make_shared<QRestAccessManager>(qnam_.get()));
    pay.pay(map);

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestPay)

#include "tst_pay.moc"
