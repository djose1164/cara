#include "keychainimpl.h"

using namespace Qt::StringLiterals;

namespace
{
constexpr auto serviceName{"io.github.djose1164.KeyChainImpl"_L1};
}

namespace Authentication
{

KeyChainImpl::KeyChainImpl(QObject *parent)
    : AbstractKeyChain(parent)
    , readerJob_{serviceName}
    , writerJob_{serviceName}
    , deleterJob_{serviceName}
{
    readerJob_.setAutoDelete(false);
    writerJob_.setAutoDelete(false);
    deleterJob_.setAutoDelete(false);
}

void KeyChainImpl::readKey(const QString &key)
{
    readerJob_.setKey(key);
    readerJob_.start();

    connect(&readerJob_, &QKeychain::ReadPasswordJob::finished, this, [=, this] {
        if (readerJob_.error() == QKeychain::NoError or readerJob_.error() == QKeychain::EntryNotFound)
        {
            qDebug() << "KeyChainImpl: restoredValue:" << readerJob_.textData();
            emit keyRestored(key, readerJob_.textData());
        }
        else if (readerJob_.error() == QKeychain::AccessDenied)
        {
            deleterJob_.setKey(key);
            deleterJob_.start();
            const QString errmsg = "Failed to read %1 key! Error (%2): %3"_L1
                                       .arg(key)
                                       .arg(QString::number(readerJob_.error()))
                                       .arg(readerJob_.errorString());
            qWarning() << "KeyChainImpl::readKey: " << errmsg;
            emit errorOccurred(Error::ReadFailed, errmsg);
        }
        else
        {
            const QString errmsg = "Failed to read %1 key! Error: %2"_L1.arg(key).arg(
                readerJob_.errorString());
            qWarning() << "KeyChainImpl::readKey: " << errmsg;
            emit errorOccurred(Error::ReadFailed, errmsg);
        }
    });
}

void KeyChainImpl::writeKey(const QString &key, const QString &value)
{
    writerJob_.setKey(key);
    writerJob_.setTextData(value);
    writerJob_.start();

    connect(&writerJob_, &QKeychain::WritePasswordJob::finished, this, [=, this] {
        if (writerJob_.error() == QKeychain::NoError)
        {
            emit keyStored(key);
        }
        else
        {
            const QString errmsg{"Failed to write %1 key!"_L1.arg(key)};
            qWarning() << "KeyChainImpl::writeKey: " << errmsg;
            emit errorOccurred(Error::WriteFailed, errmsg);
        }
    });
}

void KeyChainImpl::deleteKey(const QString &key)
{
    deleterJob_.setKey(key);
    deleterJob_.start();

    connect(&deleterJob_, &QKeychain::DeletePasswordJob::finished, this, [=, this] {
        if (deleterJob_.error() == QKeychain::NoError)
        {
            emit keyDeleted(key);
        }
        else
        {
            const QString errmsg{"Failed to delete %1 key!"_L1.arg(key)};
            qWarning() << "KeyChainImpl::deleteKey: " << errmsg;
            emit errorOccurred(Error::DeleteFailed, errmsg);
        }
    });
}

} // namespace Authentication
