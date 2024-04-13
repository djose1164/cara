#ifndef QNAMSINGLETON_H
#define QNAMSINGLETON_H

#include <QPointer>

class QNetworkAccessManager;

class QNAMSingleton
{
public:
    static QNAMSingleton &instance();

    QNAMSingleton(const QNAMSingleton &) = delete;
    QNAMSingleton operator=(const QNAMSingleton &) = delete;

    QNetworkAccessManager *qnam() const;

private:
    QNAMSingleton();
    ~QNAMSingleton() = default;

    std::shared_ptr<QNetworkAccessManager> qnam_;
};

#endif // QNAMSINGLETON_H
