#ifndef KEYCHAINIMPL_H
#define KEYCHAINIMPL_H

#include "abstractkeychain.h"
#include <qtkeychain/keychain.h>
#include <QQmlEngine>

namespace Authentication
{

class KeyChainImpl : public AbstractKeyChain
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Keychain)
public:
    explicit KeyChainImpl(QObject *parent = nullptr);

    // AbstractKeyChain interface
    void readKey(const QString &key) override;
    void writeKey(const QString &key, const QString &value) override;
    void deleteKey(const QString &key) override;

private:
    QKeychain::ReadPasswordJob readerJob_;
    QKeychain::WritePasswordJob writerJob_;
    QKeychain::DeletePasswordJob deleterJob_;
};

} // namespace Authentication

#endif // KEYCHAINIMPL_H
