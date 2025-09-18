#include "androidkeychain.h"

#include <qaesencryption.h>
#include <qtkeychain/keychain.h>

#include <QRandomGenerator>
#include <QSettings>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto serviceName{"io.github.djose1164.AndroidKeyChain"_L1};
constexpr auto AESKeyField{"tokenRefreshAESKey"_L1};
constexpr auto section{"AndroidKeyChain"_L1};
}

namespace Authentication
{

AndroidKeyChain::AndroidKeyChain(QObject *parent)
    : AbstractKeyChain(parent)
    , readerJob_{}
    , writerJob_{}
    , deleterJob_{}
    , aesKey_{}
{
}

void AndroidKeyChain::readKey(const QString &key)
{
    const auto readerHandle = [this, key] {
        QSettings settings;
        QString stored{QSettings().value(key).toString()};
        if (stored.isEmpty())
        {
            emit errorOccurred(Error::ReadFailed, "Stored value is empty!");
            return;
        }

        QByteArray combined{QByteArray::fromBase64(stored.toUtf8())};
        QByteArray iv{combined.first(16)};
        QByteArray ciphertext{combined.sliced(16)};
        qDebug() << "AndroidKeyChain::readKey: combined: " << combined;
        qDebug() << "AndroidKeyChain::readKey: iv: " << iv;
        qDebug() << "AndroidKeyChain::readKey: ciphertext: " << ciphertext;

        QAESEncryption aesEncrypt{QAESEncryption::AES_256, QAESEncryption::CBC};
        const auto AESKey{QByteArray::fromBase64(aesKey_.toUtf8())};
        const QString dectyptedText = aesEncrypt.decode(ciphertext, AESKey, iv);
        qDebug() << "AndroidKeyChain::readKey: dectyptedText: " << dectyptedText;
        emit keyRestored(key, dectyptedText);
    };

    if (not aesKey_.isEmpty())
    {
        readerHandle();
    }
    else
    {
        readAESKey(readerHandle);
    }
}

void AndroidKeyChain::writeKey(const QString &key, const QString &value)
{
    const auto saveKey = [this, key, value] {
        const auto AESKey{QByteArray::fromBase64(aesKey_.toUtf8())};
        Q_ASSERT(AESKey.size() == 32);
        if (aesKey_.isEmpty())
        {
            qWarning() << "AndroidKeyChain::writeKey: the AES key is empty!";
            return;
        }

        qDebug() << "AndroidKeyChain::writeKey: enconding " << AESKey;
        qDebug() << "AndroidKeyChain::writeKey: enconding " << key;
        QAESEncryption aesEncrypt{QAESEncryption::AES_256, QAESEncryption::CBC};
        QByteArray iv{generateInitVector()};
        Q_ASSERT(iv.size() == 16);
        QByteArray ciphertext = aesEncrypt.encode(value.toUtf8(), AESKey, iv);
        Q_ASSERT(not ciphertext.isEmpty());

        qDebug() << "AndroidKeyChain::writeKey: value: " << value.sliced(0, 20);
        qDebug() << "AndroidKeyChain::writeKey: ciphertext: " << ciphertext.toBase64().sliced(0, 20);

        QByteArray combined{iv + ciphertext};
        QSettings().setValue(key, QString{combined.toBase64()});
        emit keyStored(key);
    };

    if (not aesKey_.isEmpty())
    {
        saveKey();
    }
    else
    {
        readAESKey(saveKey);
    }
}

void AndroidKeyChain::deleteKey(const QString &key)
{
    deleterJob_ = new QKeychain::DeletePasswordJob(serviceName, this);
    deleterJob_->setKey(key);
    deleterJob_->start();

    connect(deleterJob_, &QKeychain::Job::finished, this, [this, key] {
        if (deleterJob_->error() != QKeychain::NoError)
        {
            qWarning() << "AndroidKeyChain::deleteKey: " << deleterJob_->errorString();
            emit errorOccurred(Error::DeleteFailed, deleterJob_->errorString());
            return;
        }

        QSettings{}.remove(key);

        emit keyDeleted(key);
    });
}

QByteArray AndroidKeyChain::generateInitVector()
{
    QByteArray iv{16, 0};
    QRandomGenerator::global()->generate(iv.begin(), iv.end());
    return iv;
}

void AndroidKeyChain::readAESKey(std::function<void()> callback)
{
    if (not aesKey_.isEmpty())
    {
        if (callback)
            callback();
        return;
    }

    readerJob_ = new QKeychain::ReadPasswordJob(serviceName, this);
    readerJob_->setKey(AESKeyField);
    readerJob_->start();

    connect(readerJob_,
            &QKeychain::Job::finished,
            this,
            [this, key = AESKeyField, callback = std::move(callback)] {
                if (readerJob_->error() == QKeychain::NoError)
                {
                    aesKey_ = readerJob_->textData();
                    if (callback)
                        callback();
                    emit keyRestored(key, aesKey_);
                    qDebug() << "AndroidKeyChain::readAESKey: AES key found! Key: " << aesKey_;
                }
                else if (readerJob_->error() == QKeychain::EntryNotFound)
                {
                    createAesKey(callback);
                }
                else
                {
                    qWarning() << "AndroidKeyChain::checkAesKeyExists: readerJob_: "
                               << readerJob_->errorString();
                    emit errorOccurred(Error::ReadFailed, readerJob_->errorString());
                }
            });
}

void AndroidKeyChain::createAesKey(std::function<void()> onSuccess)
{
    writerJob_ = new QKeychain::WritePasswordJob(serviceName, this);
    writerJob_->setKey(AESKeyField);

    QByteArray newKey{32, Qt::Uninitialized};
    QRandomGenerator::global()->generate(newKey.begin(), newKey.end());

    aesKey_ = newKey.toBase64();
    writerJob_->setTextData(aesKey_);

    connect(writerJob_,
            &QKeychain::Job::finished,
            this,
            [this, onSuccess = std::move(onSuccess)] {
                if (writerJob_->error() != QKeychain::NoError)
                {
                    qWarning() << "AndroidKeyChain::checkAesKeyExists: writerJob_: "
                               << writerJob_->errorString();
                    return;
                }

                qDebug() << "AndroidKeyChain::createAesKey: The AES key " << aesKey_
                         << "was created!";
                if (onSuccess)
                    onSuccess();

                qDebug() << "AndroidKeyChain::createAesKey...Doe!";
            });

    writerJob_->start();
}

} // namespace Authentication
