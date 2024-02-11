#include "Authentication/data/restuserauthentication.h"
#include "Authentication/entity/credential.h"
#include "Authentication/login.h"
#include "Catalogue/productrepository.h"
#include "User/data/localuserstore.h"
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
}

void TestProductRepository::init()
{
    Authentication::Business::Login login{};
    QSignalSpy spy{&login, &Authentication::Business::Login::authenticated};

    QVERIFY(spy.isValid());

    Authentication::Data::RestUserAuthentication restAuthenticator{};
    Authentication::Entity::Credential credentials{};
    credentials.setUsername("cara");
    credentials.setPassword("1234");
    restAuthenticator.setCredentials(credentials);
    login.setAuthenticator(&restAuthenticator);
    User::Data::LocalUserStore lus{};
    login.setStore(&lus);

    login.authenticate();
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
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
