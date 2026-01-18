#include "abstractsupplierrepository.h"
#include "restsupplierrepository.h"
#include <QObject>
#include <QTest>
#include <QSignalSpy>
#include <QJsonObject>

using namespace ProductInventory;

class TestRestSupplierRepository : public QObject
{
    Q_OBJECT
public:
    explicit TestRestSupplierRepository(QObject *parent = nullptr);

private slots:
    void tst_shouldEmitSuppliersFetchedWhenSuppliersAreFetched();
    void tstt_shouldReturnTrueIfFetchedSuppliersAreValid();
};

TestRestSupplierRepository::TestRestSupplierRepository(QObject *parent)
    : QObject{parent}
{
}

void TestRestSupplierRepository::tst_shouldEmitSuppliersFetchedWhenSuppliersAreFetched()
{
    std::unique_ptr<ProductInventory::AbstractSupplierRepository> repository;
    repository = std::make_unique<ProductInventory::RestSupplierRepository>();

    QSignalSpy spy{repository.get(), &ProductInventory::AbstractSupplierRepository::suppliersFetched};
    QVERIFY(spy.isValid());

    repository->fetchSuppliers();
    QVERIFY(spy.wait());

    QVERIFY(spy.count() == 1);
}

void TestRestSupplierRepository::tstt_shouldReturnTrueIfFetchedSuppliersAreValid()
{
    std::unique_ptr<ProductInventory::AbstractSupplierRepository> repository;
    repository = std::make_unique<ProductInventory::RestSupplierRepository>();

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

    QVERIFY(spy.wait());

}

QTEST_MAIN(TestRestSupplierRepository)

#include "tst_restsupplierrepository.moc"
