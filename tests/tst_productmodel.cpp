#include "Inventory/inventoryrepository.h"
#include "Inventory/model/productmodel.h"
#include "loginhelper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>

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
};



TestProductModel::TestProductModel(QObject *parent)
    : QObject{parent}
{

}

void TestProductModel::initTestCase()
{
    LoginHelper::login();
}

void TestProductModel::init()
{

}

void TestProductModel::test_shouldEmitUpdatedAfterUpdating() const
{
    Model::ProductModel pm{};
    Data::InventoryRepository repository{};
    pm.setRepository(&repository);

    QSignalSpy spy{&pm, &Model::ProductModel::updated};

    pm.update(1);
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

void TestProductModel::test_shouldPopulateModelIfCatalogIsNotEmpty() const
{
    Model::ProductModel pm{};
    Data::InventoryRepository repository{};
    pm.setRepository(&repository);

    QSignalSpy spy{&repository, &Data::InventoryRepository::fetchedAll};

    pm.update(1);
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
    QVERIFY(pm.count() > 0);
}

QTEST_MAIN(TestProductModel)

#include "tst_productmodel.moc"
