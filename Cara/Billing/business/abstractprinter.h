#ifndef ABSTRACTPRINTER_H
#define ABSTRACTPRINTER_H

#include <QObject>
#include <QQmlEngine>

template <typename T>
concept Printable = requires(T printer)
{
    {printer.print()} -> std::same_as<void>;
};

namespace Billing
{

class AbstractDocument;
class FilenameGenerator;
class AbstractPrinter : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    using QObject::QObject;

    virtual ~AbstractPrinter() = default;
    virtual void print() = 0;
    virtual void setFilenameGenerator(FilenameGenerator *generator) = 0;
    virtual void setDocument(AbstractDocument *document) = 0;

signals:
    void orderPrinted(const QString &path);
};
}

#endif // ABSTRACTPRINTER_H
