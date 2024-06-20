#ifndef TOKENSTORE_H
#define TOKENSTORE_H

#include "istore.h"

#include <QProperty>

namespace Authentication::Data
{
class TokenStore : public Store::IStore
{
    Q_OBJECT
    Q_PROPERTY(QString refresh READ refresh WRITE setRefresh BINDABLE bindableRefreshToken FINAL)
    Q_PROPERTY(QString access READ access WRITE setAccess NOTIFY accessChanged FINAL)
    QML_ELEMENT
public:
    explicit TokenStore(QObject *parent = nullptr);

    // IStore interface
    bool save(const QByteArray &data) override;

    QString refresh() const override;
    void setRefresh(const QString &newRefresh);
    QBindable<QString> bindableRefreshToken() const override;

    QString access() const;
    void setAccess(const QString &newAccess);

public slots:
    void restore() override;
    void clear() override;

signals:
    void credentialsChanged();

    void accessChanged();

private:
    QProperty<QString> refresh_;
    QString access_;
};
}

#endif // TOKENSTORE_H
