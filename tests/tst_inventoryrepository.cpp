#include "Inventory/inventoryrepository.h"
#include "loginhelper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>

class TestInventoryRepository : public QObject
{
    Q_OBJECT
public:
    explicit TestInventoryRepository(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void init();

    void test_shouldEmitFetchedAllUponFetchingAll() const;
};



TestInventoryRepository::TestInventoryRepository(QObject *parent)
    : QObject{parent}
{

}

void TestInventoryRepository::initTestCase()
{
    LoginHelper::login();
}

void TestInventoryRepository::init()
{

}

void TestInventoryRepository::test_shouldEmitFetchedAllUponFetchingAll() const
{
    Data::InventoryRepository repository{};

    QSignalSpy spy{&repository, &Data::InventoryRepository::fetchedAll};

    repository.fetchAll(1);
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestInventoryRepository)

#include "tst_inventoryrepository.moc"
