#include "Authentication/login.h"
#include "Customer/data/customerresource.h"
#include "Authentication/entity/credential.h"
#include "Authentication/data/restuserauthentication.h"
#include "restservice.h"
#include <QTest>
#include <QSignalSpy>
#include <QJsonDocument>
#include <QJsonObject>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto customerPath{"customers"_L1};
}

class TestCustomerResource : public QObject
{
    Q_OBJECT
public:
    explicit TestCustomerResource(QObject *parent = nullptr);

private slots:
    void initTestCase();

    void test_shouldReturnCustomersByTheirSalespersonId() const;
    void test_shouldEmitContentNotFoundIfSalespersonidIsNotValid() const;
};



TestCustomerResource::TestCustomerResource(QObject *parent)
    : QObject{parent}
{

}

void TestCustomerResource::initTestCase()
{
    QCoreApplication::setOrganizationDomain("cara.com");
    QCoreApplication::setOrganizationName("Cara");
    QCoreApplication::setApplicationName("CaraTesting");

    Authentication::Business::Login login{};
    QSignalSpy spy{&login, &Authentication::Business::Login::authenticated};

    QVERIFY(spy.isValid());

    Authentication::Data::RestUserAuthentication restAuthenticator{};
    Authentication::Entity::Credential credentials{};
    credentials.setUsername("josevictoriano");
    credentials.setPassword("2077120");
    restAuthenticator.setCredentials(credentials);
    login.setAuthenticator(&restAuthenticator);

    login.authenticate();
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}


void TestCustomerResource::test_shouldReturnCustomersByTheirSalespersonId() const
{
    Cara::RestService rs{};
    Data::CustomerResource repository{};
    repository.setPath(customerPath);

    rs.append(&repository);

    QSignalSpy spy{&repository, &Data::CustomerResource::fetched};

    QVERIFY(spy.isValid());

    repository.readBySalespersonId(1);

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);

    const QString errorString{repository.errorString()};

    QVERIFY2(errorString.isEmpty(), QString("An error has occurred: %1").arg(errorString).toUtf8());
}

void TestCustomerResource::test_shouldEmitContentNotFoundIfSalespersonidIsNotValid() const
{
    Cara::RestService rs{};
    Data::CustomerResource repository{};
    repository.setPath(customerPath);

    rs.append(&repository);

    QSignalSpy spy{&repository, &Data::CustomerResource::contentNotFound};

    QVERIFY(spy.isValid());

    repository.readBySalespersonId(0);

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);

    const QString errorString{repository.errorString()};

    QVERIFY(errorString.isEmpty() == false);
}

QTEST_MAIN(TestCustomerResource)

#include "tst_customerresource.moc"
