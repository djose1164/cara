#include <QTest>

#include "calculator.h"
#include "profitcalculator.h"

class TestProfitCalculator : public QObject
{
    Q_OBJECT
public:
    explicit TestProfitCalculator(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void init();

    void test_shouldCalculateTotalProfitCorrectly();
    void test_shouldCalculateTotalProfitAsZeroWhenNoProducts();
    void test_shouldCalculateTotalProfitCorrectlyWithSingleProduct();

private:
    std::unique_ptr<Billing::ProfitCalculator> calculator_;
    std::unique_ptr<QAbstractListModel> model_;
};

TestProfitCalculator::TestProfitCalculator(QObject *parent) {}

void TestProfitCalculator::initTestCase()
{
    model_ = std::make_unique<ProductInventory::ProductModel>();
}

void TestProfitCalculator::init()
{
    using namespace EntityLayer;

    constexpr int supplierId{1};
    constexpr int percentage{40};

    calculator_ = std::make_unique<Billing::ProfitCalculator>();
    calculator_->setModel(model_.get());

    const auto productModel{qobject_cast<ProductInventory::ProductModel *>(model_.get())};
    productModel->updateFromList(
        QList<Product>{Product{1, 100, 5}, Product{2, 200, 6}, Product{3, 180, 2}});
}

void TestProfitCalculator::test_shouldCalculateTotalProfitCorrectly()
{
    using namespace EntityLayer;

    constexpr double multi{.8};
    constexpr int percentage{40};
    calculator_->setPercentage(percentage);
    // calculator_->setProductList(QList<Product *>{new Product(1, 100 * multi, 0),
    //                                              new Product(2, 200 * multi, 0),
    //                                              new Product(3, 180 * multi, 0)});

    calculator_->calculate();

    QCOMPARE(calculator_->total(), 164.8);
}

void TestProfitCalculator::test_shouldCalculateTotalProfitAsZeroWhenNoProducts()
{
    using namespace EntityLayer;

    constexpr int percentage{40};
    calculator_->setPercentage(percentage);

    calculator_->calculate();

    QCOMPARE(calculator_->total(), 0);
}

void TestProfitCalculator::test_shouldCalculateTotalProfitCorrectlyWithSingleProduct()
{
    using namespace EntityLayer;

    constexpr double multi{.8};
    constexpr int wholePercentage{80};
    constexpr double percentage{wholePercentage / 100.0};

    const auto productModel{qobject_cast<ProductInventory::ProductModel *>(model_.get())};
    productModel->updateFromList(QList{Product{1, 100, 5}});

    calculator_->setPercentage(wholePercentage);
    // calculator_->setProductList(QList<Product *>{new Product(1, 100 * multi, 0)});

    calculator_->calculate();

    constexpr double expectedTotal{((100 - (100 * multi)) * 5) * percentage};
    QCOMPARE(calculator_->total(), expectedTotal);
}

QTEST_MAIN(TestProfitCalculator)

#include "tst_profitcalculator.moc"
