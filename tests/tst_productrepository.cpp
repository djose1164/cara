#include "localuserstore.h"
#include "login.h"
#include "productrepository.h"
#include <QTest>
#include <QSignalSpy>
#include <QJsonDocument>
#include <QJsonObject>

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
    QCoreApplication::setOrganizationDomain("cara.com");
    QCoreApplication::setOrganizationName("Cara");
    QCoreApplication::setApplicationName("CaraTesting");

    Business::Login login{};
    login.setUsername("josevictoriano");
    login.setPassword("2077120");

    Business::LocalUserStore userStore{};
    QSignalSpy spy{&login, SIGNAL(authenticated(void))};
    login.setUserStore(&userStore);

    login.authenticateCredentials();
    QVERIFY(spy.wait());
}

void TestProductRepository::init()
{

}

void TestProductRepository::test_shouldEmitAddedIfAddingSuccess() const
{
    Catalogue::ProductRepository repository{};
    QSignalSpy spy{&repository, &Catalogue::ProductRepository::added};

    QVERIFY(spy.isValid());

    constexpr QLatin1StringView filepath{XSTRING(CURRENT_SOURCE_DIR)};
    constexpr auto filename{"/tesla-model-s.jpeg"_L1};

    const QVariantMap map{
        {"name", "Carro rojo"},
        {"description", ""},
        {"buy_price", 200},
        {"sell_price", 300},
        {"image_url", QUrl::fromLocalFile(filepath + filename)},
        {"category_id", 1}
    };
    repository.add(map);

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestProductRepository)

#include "tst_productrepository.moc"
