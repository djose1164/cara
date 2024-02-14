#include "Billing/data/restorderrepository.h"
#include "User/data/localuserstore.h"
#include "Authentication/data/restuserauthentication.h"
#include "Authentication/entity/credential.h"
#include "Authentication/login.h"
#include <QTest>
#include <QSignalSpy>
#include <QJsonDocument>
#include <QJsonObject>

using namespace Qt::StringLiterals;

class TestRestOrderRepository : public QObject
{
    Q_OBJECT
public:
    explicit TestRestOrderRepository(QObject *parent = nullptr);

    QByteArray readJsonFile(const QLatin1StringView path) const;

private slots:
    void initTestCase();
    void init();

    void test_shouldEmitResourceAddedIfUsingGoodOrder() const;
    void test_shouldEmitErrorOccurredIfUsingBadOrder() const;

private:
    QJsonObject goodOrder_;
    QJsonObject badOrder_;
};



TestRestOrderRepository::TestRestOrderRepository(QObject *parent)
    : QObject{parent}
{

}

QByteArray TestRestOrderRepository::readJsonFile(const QLatin1StringView path) const
{
    QFile file{path};

    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "couldn't open file.";
        return {};
    };

    const auto jsonArrayByte{file.readAll()};
    file.close();

    return jsonArrayByte;
}

void TestRestOrderRepository::initTestCase()
{
    QCoreApplication::setOrganizationDomain("cara.com");
    QCoreApplication::setOrganizationName("Cara");
    QCoreApplication::setApplicationName("CaraTesting");

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

void TestRestOrderRepository::init()
{

}

void TestRestOrderRepository::test_shouldEmitResourceAddedIfUsingGoodOrder() const
{
    Data::RestOrderRepository repository{};
    QSignalSpy spy{&repository, &Data::RestOrderRepository::resourceAdded};

    const auto jsonDoc{QJsonDocument::fromJson(readJsonFile(":/good_order.json"_L1))};

    QVERIFY(jsonDoc.isObject());

    const auto jsonObject{jsonDoc.object()};

    QVERIFY(jsonObject.isEmpty() == false);

    repository.add(jsonObject.toVariantMap());
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

void TestRestOrderRepository::test_shouldEmitErrorOccurredIfUsingBadOrder() const
{
    Data::RestOrderRepository repository{};
    QSignalSpy spy{&repository, &Data::RestOrderRepository::errorOccurred};

    const auto jsonDoc{QJsonDocument::fromJson(readJsonFile(":/bad_order.json"_L1))};

    QVERIFY(jsonDoc.isObject());

    const auto jsonObject{jsonDoc.object()};

    QVERIFY(jsonObject.isEmpty() == false);

    repository.add(jsonObject.toVariantMap());
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestRestOrderRepository)

#include "tst_restorderrepository.moc"
