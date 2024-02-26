#ifndef PDFORDERPRINTER_H
#define PDFORDERPRINTER_H

#include <QJsonObject>
#include <QObject>
#include <QQmlEngine>

class QPrinter;
class QTextDocument;

namespace Billing
{
class AbstractDocument;
class PdfOrderPrinter : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged FINAL)
    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged FINAL)
    Q_PROPERTY(AbstractDocument *document READ document WRITE setDocument NOTIFY
                   documentChanged FINAL)
public:
    enum class Status
    {
        Printing,
        Idle
    };
    Q_ENUM(Status);

    explicit PdfOrderPrinter(QObject *parent = nullptr);

    QString path() const;
    void setPath(const QString &newPath);

    Status status() const;
    void setStatus(const Status &newStatus);

    QJsonObject order() const;
    void setOrder(const QJsonObject &newOrder);

    AbstractDocument *document() const;
    void setDocument(AbstractDocument *newDocument);

public slots:
    void print();

signals:
    void pathChanged();

    void statusChanged();

    void orderChanged();

    void orderPrinted(const QString &path);

    void documentChanged();

private:
    void printToPdf();

    std::unique_ptr<QPrinter> preparePrinter(QAnyStringView filename) const;

    std::unique_ptr<QTextDocument> prepareDocument(const QSizeF &pageSize) const;

    QString getUniqueFilename() const;

private:
    QString path_;
    Status status_;
    AbstractDocument *document_;
};
}

#endif // PDFORDERPRINTER_H
