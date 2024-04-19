#ifndef SHAREUTILS_H
#define SHAREUTILS_H

#include "shareplataform.h"

#include <QObject>
#include <QQmlEngine>

namespace Utils
{
class ShareUtils : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit ShareUtils(QObject *parent = nullptr);

public slots:
    void share(const QUrl &path);

private:
    std::unique_ptr<SharePlataform> sharePlataform_;
};
}

#endif // SHAREUTILS_H
