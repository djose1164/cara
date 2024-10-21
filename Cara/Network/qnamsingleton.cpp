#include "qnamsingleton.h"

#include <QNetworkAccessManager>
#include <QRestAccessManager>

QNAMSingleton &QNAMSingleton::instance()
{
    static QNAMSingleton qnamSingleton;
    return qnamSingleton;
}

QNAMSingleton::QNAMSingleton()
    : qnam_{std::make_shared<QNetworkAccessManager>()}
{
    qnam_->setAutoDeleteReplies(true);
}

QNetworkAccessManager *QNAMSingleton::qnam() const
{
    return qnam_.get();
}

std::unique_ptr<QRestAccessManager> QNAMSingleton::qram()
{
    return std::make_unique<QRestAccessManager>(QNAMSingleton::instance().qnam());
}
