#include "catalogproductrepository.h"
#include "productmodel.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto productIdField{"product_id"_L1};
constexpr auto quantityField{"quantity"_L1};
}

class TestProductModel : public QObject
{
    Q_OBJECT
public:
    explicit TestProductModel(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void init();

    void test_shouldEmitUpdatedAfterUpdating() const;
    void test_shouldPopulateModelIfCatalogIsNotEmpty() const;
    void test_shouldReturnTrueWhenModelContainsProduct() const;
    void test_shouldReturnFalseWhenModelNotContainsProduct() const;
    void test_shouldReturnTrueIfToJsonArrayContainsAllItems() const;
};



TestProductModel::TestProductModel(QObject *parent)
    : QObject{parent}
{

}

void TestProductModel::initTestCase()
{
    //LoginHelper::login();
}

void TestProductModel::init()
{

}

void TestProductModel::test_shouldEmitUpdatedAfterUpdating() const
{
    ProductInventory::ProductModel pm{};
    ProductInventory::CatalogProductRepository repository{};
    pm.setRepository(&repository);

    QSignalSpy spy{&pm, &ProductInventory::ProductModel::updated};

    pm.update();
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

void TestProductModel::test_shouldPopulateModelIfCatalogIsNotEmpty() const
{
    ProductInventory::ProductModel pm{};
    ProductInventory::CatalogProductRepository repository{};
    pm.setRepository(&repository);

    QSignalSpy spy{&pm, &ProductInventory::ProductModel::updated};

    pm.update();
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
    QVERIFY(pm.count() > 0);
}

void TestProductModel::test_shouldReturnTrueWhenModelContainsProduct() const
{
    ProductInventory::ProductModel pm{};
    EntityLayer::Product p{};
    p.setName("Test Product");
    p.setProductId(2);

    pm.append(p);

    QVERIFY(pm.contains(p));
}

void TestProductModel::test_shouldReturnFalseWhenModelNotContainsProduct() const
{
    ProductInventory::ProductModel pm{};
    EntityLayer::Product inModel{};
    inModel.setName("Test Product");
    inModel.setProductId(2);
    EntityLayer::Product notInModel{};
    notInModel.setName("Product ausent");
    notInModel.setProductId(4);

    pm.append(inModel);

    QVERIFY(pm.contains(notInModel) == false);
}

void TestProductModel::test_shouldReturnTrueIfToJsonArrayContainsAllItems() const
{
    QJsonArray arr;
    arr.append(QJsonObject{{productIdField, 1}, {quantityField, 4}});
    arr.append(QJsonObject{{productIdField, 4}, {quantityField, 2}});
    ProductInventory::ProductModel pm;
    EntityLayer::Product p;
    p.setProductId(1);
    p.setSelectedQty(4);
    pm.append(std::move(p));
    EntityLayer::Product p1;
    p1.setProductId(4);
    p1.setSelectedQty(2);
    pm.append(std::move(p1));

    QJsonArray res = pm.toJsonArray();

    QCOMPARE(pm.get(0).toJsonObject(), res[0].toObject());
    QCOMPARE(pm.get(1).toJsonObject(), res[1].toObject());
}

QTEST_MAIN(TestProductModel)

#include "tst_productmodel.moc"
