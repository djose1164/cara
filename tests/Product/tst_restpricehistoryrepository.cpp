#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>
#include "pricehistoryrepository.h"
#include "restpricehistoryrepository.h"

using namespace Qt::StringLiterals;

class TestProductRepository : public QObject
{
    Q_OBJECT
public:
    explicit TestProductRepository(QObject *parent = nullptr);

private slots:
    void tst_shouldEmitPriceHistoryFetchedSignalWhenPriceHistoryIsFetched();
    void tst_returnsTrueWhenPriceHistoryFetchedSignalEmitsValidData();

private:
    std::unique_ptr<Product::PriceHistoryRepository> repository_;
};

TestProductRepository::TestProductRepository(QObject *parent)
{

}

void TestProductRepository::tst_shouldEmitPriceHistoryFetchedSignalWhenPriceHistoryIsFetched()
{
    constexpr int productId{2};
    repository_ = std::make_unique<Product::RestPriceHistoryRepository>();
    QSignalSpy spy{repository_.get(), &Product::PriceHistoryRepository::fetched};

    QVERIFY(spy.isValid());
    repository_->fetchPriceHistoryBy(productId);
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

void TestProductRepository::tst_returnsTrueWhenPriceHistoryFetchedSignalEmitsValidData()
{
    constexpr int productId{2};
    repository_ = std::make_unique<Product::RestPriceHistoryRepository>();
    QSignalSpy spy{repository_.get(), &Product::PriceHistoryRepository::fetched};

    QVERIFY(spy.isValid());

    connect(repository_.get(),
            &Product::PriceHistoryRepository::fetched,
            this,
            [](const QByteArray &data)
            {
                QVERIFY(data.isEmpty() == false);
                const auto json{QJsonDocument::fromJson(data)};

                QVERIFY(json.isArray());

                const auto priceHistoryArray = json.array();
                QVERIFY(priceHistoryArray.count() > 0);

                constexpr auto verifyContainsKey = [](const auto &entityObject, QAnyStringView key)
                { QVERIFY(entityObject.contains("id"_L1)); };
                for (const auto &entity : std::as_const(priceHistoryArray))
                {
                    QVERIFY(entity.isObject());

                    const auto entityObject{entity.toObject()};

                    verifyContainsKey(entityObject, "id"_L1);
                    verifyContainsKey(entityObject, "price"_L1);
                    verifyContainsKey(entityObject, "start_date"_L1);
                    verifyContainsKey(entityObject, "thru_date"_L1);
                    verifyContainsKey(entityObject, "product_id"_L1);
                }

            });

    repository_->fetchPriceHistoryBy(productId);
    QVERIFY(spy.wait());
}

QTEST_MAIN(TestProductRepository)

#include "tst_restpricehistoryrepository.moc"
