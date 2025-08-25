#include "../loginhelper.h"
#include "productrepository.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>

using namespace Qt::StringLiterals;

#define STRING(x) (#x)
#define XSTRING(x) STRING(x)

class TestProductRepository : public QObject
{
    Q_OBJECT
public:
    explicit TestProductRepository(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void init();

    void test_shouldEmitAddedIfAddingSuccess() const;
};



TestProductRepository::TestProductRepository(QObject *parent)
    : QObject{parent}
{

}

void TestProductRepository::initTestCase()
{
    LoginHelper::login();
}

void TestProductRepository::init()
{

}

void TestProductRepository::test_shouldEmitAddedIfAddingSuccess() const
{
    ProductInventory::ProductRepository repository{};
    QSignalSpy spy{&repository, &ProductInventory::ProductRepository::added};

    QVERIFY(spy.isValid());

    constexpr QLatin1StringView filepath{XSTRING(CURRENT_SOURCE_DIR)};
    constexpr auto filename{"/tesla-model-s.jpeg"_L1};

    QVariantList images{};
    images.append(QVariantMap{{"image_url"_L1, QUrl::fromLocalFile(filepath + filename)}});
    const QVariantMap map{
        {"name", "Carro rojo"},
        {"description", ""},
        {"buy_price", 200},
        {"sell_price", 300},
        {"images", images},
        {"category_id", 1}
    };
    repository.add(map);

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestProductRepository)

#include "tst_productrepository.moc"
