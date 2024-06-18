#include "qnamsingleton.h"

#include <QNetworkAccessManager>

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
