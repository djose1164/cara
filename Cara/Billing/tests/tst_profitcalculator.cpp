#include <QTest>

#include "productmodel.h"
#include "profitcalculator.h"

namespace
{
constexpr int percentage{40};
}

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
    std::unique_ptr<Billing::Calculator> calculator_;
    std::unique_ptr<QAbstractItemModel> model_;
};

TestProfitCalculator::TestProfitCalculator(QObject *parent) {}

void TestProfitCalculator::initTestCase()
{

}

void TestProfitCalculator::init()
{
    auto profitCalc{std::make_unique<Billing::ProfitCalculator>()};
    profitCalc->setPercentage(percentage);
    calculator_ = std::move(profitCalc);

    model_ = std::make_unique<ProductInventory::ProductModel>();
    calculator_->setModel(model_.get());

    using namespace EntityLayer;
    const auto productModel{qobject_cast<ProductInventory::ProductModel *>(model_.get())};
    productModel->setProducts(
        QList<Product>{Product{1, 100, 5}, Product{2, 200, 6}, Product{3, 180, 2}});
}

void TestProfitCalculator::test_shouldCalculateTotalProfitCorrectly()
{
    using namespace EntityLayer;

    constexpr double multi{percentage/100.0};

    calculator_->calculate();

    constexpr auto expectedResult{(100*5 + 200*6 + 180*2)*multi};
    QCOMPARE(calculator_->total(), expectedResult);
}

void TestProfitCalculator::test_shouldCalculateTotalProfitAsZeroWhenNoProducts()
{
    auto pm{std::make_unique<ProductInventory::ProductModel>()};
    calculator_->setModel(pm.get());

    calculator_->calculate();

    QCOMPARE(calculator_->total(), 0);
}

void TestProfitCalculator::test_shouldCalculateTotalProfitCorrectlyWithSingleProduct()
{
    using namespace EntityLayer;

    constexpr double multi{percentage/100.0};

    const auto productModel{qobject_cast<ProductInventory::ProductModel *>(model_.get())};
    productModel->setProducts(QList{Product{1, 100, 5}});

    // calculator_->setProductList(QList<Product *>{new Product(1, 100 * multi, 0)});

    calculator_->calculate();

    constexpr double expectedTotal{(100*5)*multi};
    QCOMPARE(calculator_->total(), expectedTotal);
}

QTEST_MAIN(TestProfitCalculator)

#include "tst_profitcalculator.moc"
