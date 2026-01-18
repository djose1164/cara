#ifndef APIENVIRONMENTSERVICE_H
#define APIENVIRONMENTSERVICE_H

#include "iapienvironmentservice.h"
#include "types/apienv.h"

#include <QSettings>


namespace Core
{

class ApiEnvironmentService : public QObject, public IApiEnvironmentService
{
    Q_OBJECT
public:
    enum class Environment
    {
        Local,
        LocalNetowrk,
        Testing,
        Production,
        Unknown
    };

    explicit ApiEnvironmentService(QObject *parent = nullptr);
    virtual ~ApiEnvironmentService();

    QString environmentName() const override;

    QUrl baseUrl() const override;

    bool isProduction() const override;

    ApiEnv getCompiledApiEnv() const;

    void setEnvironment(const QVariantMap &map) override;

    QUrl uploadsPath() const;

    void setUploadsPath(const QUrl &newUploadsPath);

signals:
    void baseUrlChanged();
    void apiEnvChanged();
    void uploadsPathChanged();

private:
    void setBaseUrl(const QUrl &newBaseUrl);
    void setEnv(ApiEnvironment newApiEnv);

    bool existsPreviousApiEnv() const;
    ApiEnv getFromSettings() const;

    void restorePrevious();
    void restoreCompiledEnv();

    QUrl uploadsUrl() const;

private:
    Environment env_;
    ApiEnvironment apiEnv_;
    QUrl baseUrl_;
    QSettings settings_;
    QUrl uploadsPath_;
};

} // namespace Core

#endif // APIENVIRONMENTSERVICE_H
