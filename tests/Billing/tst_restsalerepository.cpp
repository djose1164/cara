#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>
#include "loginhelper.h"
#include "restsalerepository.h"
#include "sale.h"
#include "salerepository.h"

using namespace Qt::StringLiterals;

class TestRestSaleRepository : public QObject
{
    Q_OBJECT
public:
    explicit TestRestSaleRepository(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void test_shouldEmitFetchedWhenFetchingSales();
    void test_shouldReturnTrueWhenMappingResponseToSales();

private:
    std::unique_ptr<Billing::SaleRepository> repository_;
};


TestRestSaleRepository::TestRestSaleRepository(QObject *parent)
    : QObject(parent)
{
}

void TestRestSaleRepository::initTestCase()
{
    LoginHelper::login();
}

void TestRestSaleRepository::test_shouldEmitFetchedWhenFetchingSales()
{
    repository_ = std::make_unique<Billing::RestSaleRepository>();
    QSignalSpy spy{repository_.get(), &Billing::SaleRepository::fetched};

    spy.isValid();
    repository_->fetchSales();
    spy.wait();

    QCOMPARE(spy.count(), 1);
}

void TestRestSaleRepository::test_shouldReturnTrueWhenMappingResponseToSales()
{
    QFile file{QStringLiteral(":/sales.json")};

    QVERIFY(file.open(QFile::ReadOnly));

    auto &&doc{QJsonDocument::fromJson(file.readAll())};

    QVERIFY(doc.isObject());
    QVERIFY(doc.object().contains("sales"_L1));
    QVERIFY(doc.object()["sales"_L1].isArray());

    const auto sales = doc.object()["sales"_L1].toArray();
    const auto salesList{Billing::RestSaleRepository::mapResponseToSales(sales)};
    for (const auto &sale : salesList)
    {
        QVERIFY(sale.saleId() > 0);
        QVERIFY(sale.customerId() > 0);
        QVERIFY(sale.createdAt().isValid());
        QVERIFY(sale.totalAmount() > 0);
        QVERIFY(sale.hasItems());

        for (const auto &item : sale.saleItems())
        {
            QVERIFY(item.isValid());
        }
    }
}

QTEST_MAIN(TestRestSaleRepository)

#include "tst_restsalerepository.moc"
