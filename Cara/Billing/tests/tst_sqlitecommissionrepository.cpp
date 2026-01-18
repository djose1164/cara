#include "commission.h"
#include "commissionmodel.h"
#include "sqlitecommissionrepository.h"
#include "sqlitedatabase.h"
#include "sqliteproductmodel.h"
#include <qsqlerror.h>
#include <QJsonDocument>
#include <QSignalSpy>
#include <QSqlQuery>
#include <QTest>

using namespace Qt::StringLiterals;

constexpr auto SELECT_PAYMENT_DATE{"select payment_date from commission where unique_code = :uniqueCode"_L1};

constexpr auto SAMPLE_COMMISSION = R"({
    "adminId": 5,
    "commissionItems": [
        {
            "product_id": 1,
            "quantity": 3,
            "supplier_id": 1
        },
        {
            "product_id": 3,
            "quantity": 4,
            "supplier_id": 1
        }
    ],
    "paymentDate": "",
    "rate": 0.5,
    "salespersonId": 1,
    "amount": 2600
})"_L1; // 2025-11-22T04:00:00.000

class TestSQLiteCommissionRepository : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();

    void test_addCommission() const;
    void test_patchCommission() const;
};

void TestSQLiteCommissionRepository::initTestCase()
{
    ProductInventory::SQLiteDatabase::initDatabase();
}

void TestSQLiteCommissionRepository::test_addCommission() const
{
    Billing::SQLiteCommissionRepository repo;

    QJsonObject sample{QJsonDocument::fromJson(QByteArray(SAMPLE_COMMISSION)).object()};
    Billing::Commission c{Billing::Commission::fromMap(sample.toVariantMap())};

    QSignalSpy spy{&repo, &Billing::SQLiteCommissionRepository::added};
    QVERIFY(spy.isValid());

    repo.addCommission(c);

    QVERIFY(spy.count() == 1);
}

void TestSQLiteCommissionRepository::test_patchCommission() const
{
    Billing::SQLiteCommissionRepository repo;

    QJsonObject sample{QJsonDocument::fromJson(QByteArray(SAMPLE_COMMISSION)).object()};
    Billing::Commission c{Billing::Commission::fromMap(sample.toVariantMap())};
    const auto newPaymentDate{QDateTime::currentDateTime()};
    c.setPaymentDate(newPaymentDate);
    c.setUniqueCode(QUuid::fromString("{d00b6fd9-b7ce-4757-86a7-01f63eef7815}"_L1));

    repo.patchCommission(c, [c] {
        QSqlQuery query;
        QVERIFY(query.prepare(SELECT_PAYMENT_DATE));
        query.bindValue(":uniqueCode"_L1, c.uniqueCode());

        if (not query.exec())
            qDebug() << query.lastError().text();
        query.next();
        const auto storedPaymentDate{query.value(0).toDateTime()};
        qDebug() << storedPaymentDate;

        QCOMPARE(storedPaymentDate, c.paymentDate());
    });
}

QTEST_MAIN(TestSQLiteCommissionRepository)

#include "tst_sqlitecommissionrepository.moc"
