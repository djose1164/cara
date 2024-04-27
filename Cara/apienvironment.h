#ifndef APIENVIRONMENT_H
#define APIENVIRONMENT_H

#include <QQmlEngine>
#include "restconfig.h"

using namespace Qt::StringLiterals;

class Api : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ApiEnvironment apiEnvironment READ getApiEnvironment CONSTANT FINAL)
    QML_ELEMENT
    QML_SINGLETON
public:
    enum class ApiEnvironment
    {
        RemoteTesting,
        LocalTesting,
        LocalNetworkTesting,
        Production
    };
    Q_ENUM(ApiEnvironment)

    static consteval ApiEnvironment getApiEnvironment()
    {
        return static_cast<ApiEnvironment>(API_ENVIRONMENT);
    };

    static constexpr QAnyStringView getApiPath()
    {
        constexpr auto apiSuffix{"/api"_L1};
        switch (Api::getApiEnvironment())
        {
            using enum Api::ApiEnvironment;
        case LocalTesting:
            return QStringLiteral(LOCAL_API_URL) + apiSuffix;
        case LocalNetworkTesting:
            return QStringLiteral(LOCAL_NETWORK_API_URL) + apiSuffix;
        case RemoteTesting:
            return QStringLiteral(REMOTE_TESTING_API_URL) + apiSuffix;
        case Production:
            return QStringLiteral(REMOTE_API_URL) + apiSuffix;
        }
    }

    Api(QObject *parent = nullptr);
};


#endif // APIENVIRONMENT_H
