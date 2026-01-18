#ifndef APPSTATE_H
#define APPSTATE_H

#include "business/apienvironmentservice.h"
#include <QObject>
#include <QQmlEngine>

namespace Core
{

class AppState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Core::ApiEnvironmentService *apiEnvironment READ apiEnvironment NOTIFY apiEnvironmentChanged FINAL)
    Q_PROPERTY(QUrl uploadsPath READ uploadsPath NOTIFY uploadsPathChanged FINAL)
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit AppState(QObject *parent = nullptr);

    ApiEnvironmentService *apiEnvironment() const;
    void setApiEnvironment(ApiEnvironmentService *newApiEnvironment);

    QUrl uploadsPath() const;

signals:
    void apiEnvironmentChanged();
    void uploadsPathChanged();

private:
    bool isOnline_;
    ApiEnvironmentService *apiEnvService_;
};

} // namespace Core

#endif // APPSTATE_H
