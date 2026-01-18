#include "business/apienvironmentservice.h"
#include "../loginhelper.h"
#include <QTest>

using namespace Core;
using namespace Qt::StringLiterals;

constexpr auto apiEnvPath{":/api.env"_L1};
constexpr auto SETTINGS_SECTION{"REMOTE_API"_L1};

class TestApiEnvironmentService : public QObject
{
    Q_OBJECT
public:
    explicit TestApiEnvironmentService(QObject *parent = nullptr);

private slots:
    void initTestCase();

    void test_isProduction() const;
    void test_environmentName() const;
    void test_baseUrl() const;
    void test_getCompiledApiEnv() const;
    void test_setEnvironment() const;
};

TestApiEnvironmentService::TestApiEnvironmentService(QObject *parent)
    : QObject{parent}
{
}

void TestApiEnvironmentService::initTestCase()
{
    LoginHelper::setAppConf();
}

void TestApiEnvironmentService::test_isProduction() const
{
    ApiEnvironmentService apiEnv{};

    QVERIFY(apiEnv.isProduction() == true);
}

void TestApiEnvironmentService::test_environmentName() const
{
    ApiEnvironmentService apiEnv{};

    QCOMPARE(apiEnv.environmentName(), "production"_L1);
}

void TestApiEnvironmentService::test_baseUrl() const
{
    ApiEnvironmentService apiEnv{};

    const QUrl baseUrl{"http://127.0.0.1:5000"_L1};

    QCOMPARE(apiEnv.baseUrl(), baseUrl);
}

void TestApiEnvironmentService::test_getCompiledApiEnv() const
{
    ApiEnvironmentService apiEnvService;

    const auto env = apiEnvService.getCompiledApiEnv();

    QCOMPARE(env.apiEnv_, ApiEnvironment::Production);
    QCOMPARE(env.baseUrl_.toString(), "https://cara-api.onrender.com"_L1);
}

void TestApiEnvironmentService::test_setEnvironment() const
{
    ApiEnvironmentService apiEnvService;
    QVariantMap map{{"apiEnv"_L1, "0"}, {"baseUrl"_L1, "http://127.0.0.1:5000"_L1}};

    apiEnvService.setEnvironment(map);

    QSettings settings;
    settings.beginGroup(SETTINGS_SECTION);
    const auto storedUrl{settings.value("baseUrl"_L1).toUrl()};
    int apiEnv{settings.value("apiEnvironment"_L1).toInt()};
    settings.endGroup();

    qDebug() << "baseUrl(): " << apiEnvService.baseUrl();

    QCOMPARE(apiEnvService.baseUrl(),  map["baseUrl"_L1].toUrl());
    QCOMPARE(storedUrl, map["baseUrl"_L1].toUrl());
    QCOMPARE(apiEnv, map["apiEnvironment"_L1].toInt());
}

QTEST_MAIN(TestApiEnvironmentService)

#include "tst_apienvironmentservice.moc"
