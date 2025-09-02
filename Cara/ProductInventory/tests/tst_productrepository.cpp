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

    auto file{QFile("./tesla-model-s.jpeg"_L1)};

    QVERIFY(file.exists());
    qDebug() << file.fileName() << "\t" << QFileInfo(file).absoluteFilePath() << "\t" << QFileInfo(file).absolutePath();

    QVariantList images{};
    images.append(QVariantMap{
        {"imageUrl"_L1, QUrl::fromLocalFile(QFileInfo(file).absoluteFilePath())}
    });
    const QVariantMap map{
        {"name"_L1, "Carro rojo"_L1},
        {"description"_L1, ""},
        {"supplierPrice"_L1, 200},
        {"supplierId"_L1, 1},
        {"price"_L1, 300},
        {"images"_L1, images},
        {"categoryId"_L1, 1}
    };
    repository.add(map);

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestProductRepository)

#include "tst_productrepository.moc"
