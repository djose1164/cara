#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>
#include <QRestAccessManager>
#include "commission.h"
#include "commissionrepository.h"
#include "loginhelper.h"
#include "restcommissionrepository.h"

using namespace Qt::StringLiterals;

class TestRestCommissionRepository : public QObject
{
    Q_OBJECT
public:
    explicit TestRestCommissionRepository(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void init();
    void test_shouldEmitFetchedWhenFetchingAllCommissions();
    void test_shouldEmitAddedWhenAddingCommission();

private:
    std::unique_ptr<Billing::CommissionRepository> repository_;
};


TestRestCommissionRepository::TestRestCommissionRepository(QObject *parent)
    : QObject(parent)
{
}

void TestRestCommissionRepository::initTestCase()
{
    LoginHelper::login();
}

void TestRestCommissionRepository::init()
{
    repository_ = std::make_unique<Billing::RestCommissionRepository>();
}

void TestRestCommissionRepository::test_shouldEmitFetchedWhenFetchingAllCommissions()
{
    QSignalSpy spy{repository_.get(), &Billing::CommissionRepository::fetched};

    spy.isValid();
    repository_->fetchCommissions();
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

void TestRestCommissionRepository::test_shouldEmitAddedWhenAddingCommission()
{
    QFile f{":/insertable_commission.json"_L1};

    QVERIFY(f.open(QFile::ReadOnly));

    QJsonDocument doc{QJsonDocument::fromJson(f.readAll())};

    QVERIFY(doc.isObject());

    QSignalSpy spy{repository_.get(), &Billing::CommissionRepository::added};

    QVERIFY(spy.isValid());
    repository_->addCommission(Billing::Commission::fromJson(doc.object()));
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestRestCommissionRepository)

#include "tst_restcommissionrepository.moc"
