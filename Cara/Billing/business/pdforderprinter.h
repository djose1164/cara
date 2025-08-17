#ifndef PDFORDERPRINTER_H
#define PDFORDERPRINTER_H

#include <QJsonObject>
#include <QObject>
#include <QPrinter>
#include <QQmlEngine>

class QPrinter;
class QTextDocument;

namespace Billing
{

class AbstractPrinter;
class AbstractDocument;
class FilenameGenerator;
class PdfOrderPrinter : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged FINAL)
    Q_PROPERTY(AbstractDocument *document READ document WRITE setDocument NOTIFY
                   documentChanged FINAL)
    Q_PROPERTY(FilenameGenerator *filenameGenerator READ filenameGenerator WRITE setFilenameGenerator NOTIFY filenameGeneratorChanged FINAL)
public:
    enum class Status
    {
        Printing,
        Idle,
        Error
    };
    Q_ENUM(Status);

    explicit PdfOrderPrinter(QObject *parent = nullptr);

    Status status() const;
    void setStatus(const Status &newStatus);

    QJsonObject order() const;
    void setOrder(const QJsonObject &newOrder);

    AbstractDocument *document() const;
    void setDocument(AbstractDocument *newDocument);

    FilenameGenerator *filenameGenerator() const;
    void setFilenameGenerator(FilenameGenerator *newFilenameGenerator);

public slots:
    void print();

signals:
    void pathChanged();

    void statusChanged();

    void orderChanged();

    void documentChanged();

    void orderPrinted(const QString &path);

    void filenameGeneratorChanged();

private:
    void printBill();

    static AbstractPrinter *createPlatformPrinter(QObject *parent = nullptr);

private:
    Status status_;
    AbstractDocument *document_;
    FilenameGenerator *filenameGenerator_;
    std::unique_ptr<QPrinter> printer_;
    AbstractPrinter *orderPrinter_;
};
}

#endif // PDFORDERPRINTER_H
