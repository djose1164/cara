#include "Billing/entity/payment.h"
#include <QTest>
#include <QSignalSpy>
#include <QJsonDocument>
#include <QJsonObject>

class TestPayment : public QObject
{
    Q_OBJECT
public:
    explicit TestPayment(QObject *parent = nullptr);

private slots:

    void test_shouldReturnTrueIfPaymentIsValid();
    void test_shouldReturnFalseIfPaymentIdIsInvalid();
    void test_shouldReturnFalseIfPaidAmountIsInvalid();
    void test_shouldReturnFalseIfAmountToPayIsInvalid();
    void test_shouldReturnFalseIfAmountIsInvalid();
    void test_shouldReturnFalseIfIsntPayable();
    void test_shouldReturnTrueIfIsPayable();
};



TestPayment::TestPayment(QObject *parent)
    : QObject{parent}
{

}

void TestPayment::test_shouldReturnTrueIfPaymentIsValid()
{
    const QVariantMap map = {
        {"paymentId", 329},
        {"paidAmount", 0},
        {"amountToPay", 1100},
        {"orderId", 333}
    };
    const auto p{Entity::Payment::fromMap(map)};

    const bool res{p.isValid()};

    QVERIFY(res);
}

void TestPayment::test_shouldReturnFalseIfPaymentIdIsInvalid()
{
    const QVariantMap map = {
        {"paidAmount", 0},
        {"amountToPay", 1100},
        {"amount", 20}
    };
    const auto p{Entity::Payment::fromMap(map)};

    const bool res{p.isValid()};

    QVERIFY(res == false);
}

void TestPayment::test_shouldReturnFalseIfPaidAmountIsInvalid()
{
    const QVariantMap map = {
        {"paymentId", 329},
        {"amountToPay", 1100},
        {"amount", 20}
    };
    const auto p{Entity::Payment::fromMap(map)};

    const bool res{p.isValid()};

    QVERIFY(res == false);
}

void TestPayment::test_shouldReturnFalseIfAmountToPayIsInvalid()
{
    const QVariantMap map = {
        {"paymentId", 329},
        {"paidAmount", 0},
        {"amount", 20}
    };
    const auto p{Entity::Payment::fromMap(map)};

    const bool res{p.isValid()};

    QVERIFY(res == false);
}

void TestPayment::test_shouldReturnFalseIfAmountIsInvalid()
{
    const QVariantMap map = {
        {"paymentId", 329},
        {"paidAmount", 0},
        {"amountToPay", 1100},
    };
    const auto p{Entity::Payment::fromMap(map)};

    const bool res{p.isValid()};

    QVERIFY(res == false);
}

void TestPayment::test_shouldReturnFalseIfIsntPayable()
{
    const QVariantMap map = {
        {"amount", 0}
    };
    const auto p{Entity::Payment::fromMap(map)};

    const bool res{p.isPayeable()};

    QVERIFY(res == false);
}

void TestPayment::test_shouldReturnTrueIfIsPayable()
{
    const QVariantMap map = {
        {"paymentId", 329},
        {"amount", 100},
    };
    const auto p{Entity::Payment::fromMap(map)};

    const bool res{p.isPayeable()};

    QVERIFY(res);
}

QTEST_MAIN(TestPayment)

#include "tst_payment.moc"
