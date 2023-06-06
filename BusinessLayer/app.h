#ifndef APP_H
#define APP_H

#include "billmodel.h"
#include "rest.h"
#include <QObject>
#include <QQmlEngine>
#include <QtQml>
#include <QString>

namespace BusinessLayer {
using ModelLayer::BillModel;

class App : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString accessToken READ accessToken WRITE setAccessToken NOTIFY accessTokenChanged)
    Q_PROPERTY(BillModel *billModel READ billModel WRITE setBillModel NOTIFY billModelChanged)

public:
    static App *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);
    static App *instance();

    QString accessToken() const;
    void setAccessToken(const QString &accessToken);
    QBindable<QString> bindableAccessToken() const;

    BillModel *billModel() const;
    void setBillModel(BillModel *const billModel);
    QBindable<BillModel> bindableBillModel() const;

signals:
    void accessTokenChanged();
    void billModelChanged();

private:
    App();
    
    static App *instance_;
    
    Q_OBJECT_BINDABLE_PROPERTY(App, QString, accessToken_, &App::accessTokenChanged);
    Q_OBJECT_BINDABLE_PROPERTY(App, BillModel *, billModel_, &App::billModelChanged);
    Rest *rest_;
};
}

#endif // APP_H
