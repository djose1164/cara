#include <QJsonObject>
#include <QTest>
#include "commission.h"

using namespace Qt::StringLiterals;

class TestCommission : public QObject
{
    Q_OBJECT
public:
    TestCommission(QObject *parent = nullptr);

private slots:
    void test_shouldReturnTrueIfCorrectlyInitialized() const;
    void test_shouldReturnTrueIfCorrectlySerialized() const;
};

TestCommission::TestCommission(QObject *parent) : QObject(parent)
{}

void TestCommission::test_shouldReturnTrueIfCorrectlyInitialized() const
{
    Billing::Commission c;

    QCOMPARE(c.rate(), 0);
    QCOMPARE(c.amount(), 0);
    QCOMPARE(c.commissionId(), 0);
    QCOMPARE(c.salespersonId(), 0);
    QCOMPARE(c.paymentDate(), QDateTime{});
    QCOMPARE(c.createdAt(), QDateTime{});
    QVERIFY(c.commissionItems().isEmpty());
}

void TestCommission::test_shouldReturnTrueIfCorrectlySerialized() const
{
    Billing::Commission c;
    c.setRate(25.30);
    c.setAmount(500);
    c.setCommissionId(3);
    c.setAdminId(3);
    c.setSalespersonId(1);
    c.setPaymentDate(QDateTime::currentDateTime());
    c.setCreatedAt(QDateTime::currentDateTime());
    c.setCommissionItems(QList<Billing::CommissionItem>
                         {
                             Billing::CommissionItem{1, 3, 50.41, 1},
                             Billing::CommissionItem{5, 2, 79.54, 1},
                             Billing::CommissionItem{2, 7, 32.48, 1},
                         });

    const QJsonObject json{c.toJson()};
    const auto paymentDate{QDateTime::fromString(json["paymentDate"_L1].toString(), Qt::ISODate)};
    const auto createdAt{QDateTime::fromString(json["createdAt"_L1].toString(), Qt::ISODate)};

    QCOMPARE(json["rate"_L1].toDouble(), 25.30);
    QCOMPARE(json["amount"_L1].toDouble(), 500);
    QCOMPARE(json["commissionId"_L1].toInt(), 3);
    QCOMPARE(json["adminId"_L1].toInt(), 3);
    QCOMPARE(json["salespersonId"_L1].toInt(), 1);
    QVERIFY(paymentDate.isValid());
    QVERIFY(createdAt.isValid());
}

QTEST_MAIN(TestCommission)

#include "tst_commission.moc"
