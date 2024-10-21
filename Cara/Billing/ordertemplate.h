#ifndef ORDERTEMPLATE_H
#define ORDERTEMPLATE_H

#include "abstractdocument.h"
#include <QJsonObject>
#include <QQmlEngine>

namespace Billing
{
class OrderTemplate : public AbstractDocument, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    QML_ELEMENT
    Q_PROPERTY(QJsonObject order READ order WRITE setOrder NOTIFY orderChanged FINAL)
public:
    explicit OrderTemplate(QObject *parent = nullptr);

    QJsonObject order() const;
    void setOrder(const QJsonObject &newOrder);

    QString style() const;

    void print(QPagedPaintDevice *printer) override;

signals:
    void orderChanged();

    void errorOccurred(const QString &errorString);

private:
    // IOrderTemplate interface
    void loadHtml();

    void loadStyle();

    QString getFormattedOrder();

    // QQmlParserStatus interface
    void classBegin() override;

    void componentComplete() override;

    std::optional<QByteArray> load(QAnyStringView filename);

    void setup();

    std::optional<QStringList> htmlDetails();

    void resetCalculatedTotal();

private:
    QString html_;
    QString style_;
    QJsonObject order_;
    unsigned calculatedTotal_;
};
}
#endif // ORDERTEMPLATE_H
