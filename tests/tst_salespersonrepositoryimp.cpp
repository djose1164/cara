#include "Salesperson/data/salespersonrepositoryimp.h"
#include "Salesperson/salesperson.h"
#include "loginhelper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>

using namespace Qt::StringLiterals;
namespace
{
constexpr auto salespersonId{1};
}

class TestSalespersonRepositoryImp : public QObject
{
    Q_OBJECT
public:
    explicit TestSalespersonRepositoryImp(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void test_shouldGetSalespersonById() const;
    void test_shouldGetCustomersByAdminId() const;
};



TestSalespersonRepositoryImp::TestSalespersonRepositoryImp(QObject *parent)
    : QObject{parent}
{

}

void TestSalespersonRepositoryImp::initTestCase()
{
    LoginHelper::login();
}

void TestSalespersonRepositoryImp::test_shouldGetSalespersonById() const
{
    Data::SalespersonRepositoryImpl repository{};
    QSignalSpy spy{&repository, SIGNAL(fetched(void))};

    QVERIFY(spy.isValid());

    repository.getSalespersonById(salespersonId);

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
    QVERIFY(repository.buffer().isEmpty() == false);

    const auto obj{
        QJsonDocument::fromJson(repository.buffer())
            .object()
    };

    QVERIFY(obj.contains("salesperson"));

    const auto doc{QJsonDocument(obj["salesperson"].toObject())};
    const auto sp{Entity::Salesperson::fromJson(doc.toJson())};

    QVERIFY(sp.isValid());
}

void TestSalespersonRepositoryImp::test_shouldGetCustomersByAdminId() const
{
    Data::SalespersonRepositoryImpl repository{};
    QSignalSpy spy{&repository, SIGNAL(fetched(void))};

    QVERIFY(spy.isValid());

    repository.getCustomers(salespersonId);

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
    QVERIFY(repository.buffer().isEmpty() == false);

    const auto obj{
        QJsonDocument::fromJson(repository.buffer())
            .object()
    };

    QVERIFY(obj.contains("customers"_L1));

    const bool result{obj["customers"_L1].isArray()};

    QVERIFY(result);
}

QTEST_MAIN(TestSalespersonRepositoryImp)

#include "tst_salespersonrepositoryimp.moc"
