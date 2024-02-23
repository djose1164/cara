#ifndef ABSTRACTDOCUMENT_H
#define ABSTRACTDOCUMENT_H

#include <QObject>
#include <QQmlEngine>

namespace Billing
{
class AbstractDocument : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    explicit AbstractDocument(QObject *parent = nullptr);
    virtual ~AbstractDocument();

    virtual void loadHtml() = 0;

    virtual void loadStyle() = 0;

    virtual QString getFormattedOrder() = 0;
    virtual QString style() const = 0;

    virtual void setOrder(const QJsonObject &newOrder) = 0;
};
}
#endif // ABSTRACTDOCUMENT_H
