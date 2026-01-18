#include "commission.h"
#include "sqlitecommissionmodel.h"
#include "sqlitedatabase.h"
#include <QJsonDocument>
#include <QSignalSpy>
#include <QTest>

using namespace Qt::StringLiterals;

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
    "paymentDate": "2025-11-22T04:00:00.000",
    "rate": 0.5,
    "salespersonId": 1
})"_L1;

class TestSQLiteCommissionModel : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void test_append() const;
};

void TestSQLiteCommissionModel::initTestCase()
{
    ProductInventory::SQLiteDatabase::initDatabase();
}

void TestSQLiteCommissionModel::test_append() const
{
    Billing::SQLiteCommissionModel commissionModel;
    ProductInventory::SQLiteProductModel productModel;
    commissionModel.setProductModel(&productModel);
    productModel.refresh();
    QJsonObject sample{QJsonDocument::fromJson(QByteArray(SAMPLE_COMMISSION)).object()};
    Billing::Commission c{Billing::Commission::fromJson(sample)};

    QSignalSpy spy{&commissionModel, &Billing::SQLiteCommissionModel::commissionAddedSuccessfully};
    QVERIFY(spy.isValid());

    commissionModel.append(sample.toVariantMap());

    QVERIFY(spy.count() == 1);
}

QTEST_MAIN(TestSQLiteCommissionModel)

#include "tst_sqlitecommissionmodel.moc"
