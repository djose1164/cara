#ifndef ANDROIDKEYCHAIN_H
#define ANDROIDKEYCHAIN_H

#include "abstractkeychain.h"

namespace QKeychain
{
class DeletePasswordJob;
class WritePasswordJob;
class ReadPasswordJob;
}
namespace Authentication
{

class AndroidKeyChain : public AbstractKeyChain
{
    Q_OBJECT
public:
    explicit AndroidKeyChain(QObject *parent = nullptr);

    // AbstractKeyChain interface
    void readKey(const QString &key) override;
    void writeKey(const QString &key, const QString &value) override;
    void deleteKey(const QString &key) override;

    static QByteArray generateInitVector();

signals:

private:
    void readAESKey(std::function<void()> callback);
    void createAesKey(std::function<void()> onSuccess);

private:
    QPointer<QKeychain::ReadPasswordJob> readerJob_;
    QPointer<QKeychain::WritePasswordJob> writerJob_;
    QPointer<QKeychain::DeletePasswordJob> deleterJob_;
    QString aesKey_;
};

} // namespace Authentication

#endif // ANDROIDKEYCHAIN_H
