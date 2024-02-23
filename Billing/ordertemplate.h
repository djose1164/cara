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
    QML_ELEMENT
    Q_PROPERTY(QJsonObject order READ order WRITE setOrder NOTIFY orderChanged FINAL)
public:
    explicit OrderTemplate(QObject *parent = nullptr);

    QJsonObject order() const;
    void setOrder(const QJsonObject &newOrder) override;

    QString style() const override;

signals:
    void orderChanged();

private:
    // IOrderTemplate interface
    void loadHtml() override;

    void loadStyle() override;

    QString getFormattedOrder() override;

    // QQmlParserStatus interface
    void classBegin() override;

    void componentComplete() override;

    std::optional<QByteArray> load(QAnyStringView filename);

    void setup();

    std::optional<QStringList> htmlDetails();

private:
    QString html_;
    QString style_;
    QJsonObject order_;
    unsigned calculatedTotal_;
};
}
#endif // ORDERTEMPLATE_H
