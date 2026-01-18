#include "abstractsupplierrepository.h"
#include "sqlitedatabase.h"
#include "sqlitesupplierrepository.h"
#include <QJsonObject>
#include <QObject>
#include <QSignalSpy>
#include <QTest>

using namespace ProductInventory;

class TestSQLiteSupplierRepository : public QObject
{
    Q_OBJECT
public:
    explicit TestSQLiteSupplierRepository(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void tst_shouldEmitSuppliersFetchedWhenSuppliersAreFetched();
    void tstt_shouldReturnTrueIfFetchedSuppliersAreValid();
};

TestSQLiteSupplierRepository::TestSQLiteSupplierRepository(QObject *parent)
    : QObject{parent}
{
}

void TestSQLiteSupplierRepository::initTestCase()
{
    SQLiteDatabase::initDatabase();
}

void TestSQLiteSupplierRepository::tst_shouldEmitSuppliersFetchedWhenSuppliersAreFetched()
{
    std::unique_ptr<ProductInventory::AbstractSupplierRepository> repository;
    repository = std::make_unique<ProductInventory::SQLiteSupplierRepository>();

    QSignalSpy spy{repository.get(), &ProductInventory::AbstractSupplierRepository::suppliersFetched};
    QVERIFY(spy.isValid());

    repository->fetchSuppliers();

    QVERIFY(spy.count() == 1);
}

void TestSQLiteSupplierRepository::tstt_shouldReturnTrueIfFetchedSuppliersAreValid()
{
    std::unique_ptr<ProductInventory::AbstractSupplierRepository> repository;
    repository = std::make_unique<ProductInventory::SQLiteSupplierRepository>();

    QSignalSpy spy{repository.get(),
                   &ProductInventory::AbstractSupplierRepository::suppliersFetched};
    QVERIFY(spy.isValid());

    repository->fetchSuppliers();
    connect(repository.get(),
            &ProductInventory::AbstractSupplierRepository::suppliersFetched,
            this,
            [](const auto &supplierList) {
                std::ranges::for_each(supplierList, [](const auto &s) {
                    QVERIFY(s.isValid());
                });
            });
}

QTEST_MAIN(TestSQLiteSupplierRepository)

#include "tst_sqlitesupplierrepository.moc"
