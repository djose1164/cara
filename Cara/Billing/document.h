#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QQmlEngine>
#include <abstractdocument.h>

namespace Billing
{
class Document : public Billing::AbstractDocument
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString html READ html WRITE setHtml NOTIFY htmlChanged FINAL)
    Q_PROPERTY(QString style READ style WRITE setStyle NOTIFY styleChanged FINAL)
public:
    explicit Document(QObject *parent = nullptr);

    // AbstractDocument interface
    void print(QPagedPaintDevice *printer) override;

    QString html() const;
    void setHtml(const QString &newHtml);

    QString style() const;
    void setStyle(const QString &newStyle);

signals:
    void htmlChanged();

    void styleChanged();

private:
    QString html_;
    QString style_;
};
}

#endif // DOCUMENT_H
