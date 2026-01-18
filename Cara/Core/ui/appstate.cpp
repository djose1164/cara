#include "appstate.h"
#include "business/apienvironmentservice.h"

using namespace Qt::StringLiterals;

namespace Core
{

AppState::AppState(QObject *parent)
    : QObject{parent}
    , apiEnvService_{new ApiEnvironmentService(this)}
{
    connect(apiEnvService_,
            &ApiEnvironmentService::uploadsPathChanged,
            this,
            &AppState::uploadsPathChanged);
}

ApiEnvironmentService *AppState::apiEnvironment() const
{
    return apiEnvService_;
}

void AppState::setApiEnvironment(ApiEnvironmentService *newApiEnvironment)
{
    if (apiEnvService_ == newApiEnvironment)
        return;
    apiEnvService_ = newApiEnvironment;
    emit apiEnvironmentChanged();
}

QUrl AppState::uploadsPath() const
{
    Q_ASSERT(apiEnvService_->uploadsPath().isValid());
    return apiEnvService_->uploadsPath();
}

} // namespace Cara
