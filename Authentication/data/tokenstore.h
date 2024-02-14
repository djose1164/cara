#ifndef TOKENSTORE_H
#define TOKENSTORE_H

#include "istore.h"

namespace Authentication::Data
{
class TokenStore : public Store::IStore
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString refresh READ refresh NOTIFY refreshChanged FINAL)
    Q_PROPERTY(QString access READ access NOTIFY accessChanged FINAL)
public:
    explicit TokenStore(QObject *parent = nullptr);

    // IStore interface
    void save(const QByteArray &data) override;

    QString refresh() const;
    void setRefresh(const QString &newRefresh);

    QString access() const;
    void setAccess(const QString &newAccess);

public slots:
    void restore() override;
    void clear() override;

signals:
    void credentialsChanged();

    void refreshChanged();

    void accessChanged();

private:
    QString refresh_;
    QString access_;
};
}

#endif // TOKENSTORE_H
