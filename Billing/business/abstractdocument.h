#ifndef ABSTRACTDOCUMENT_H
#define ABSTRACTDOCUMENT_H

#include <QObject>
#include <QPagedPaintDevice>
#include <QQmlEngine>
#include <QSizeF>

namespace Billing
{
class AbstractDocument : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    explicit AbstractDocument(QObject *parent = nullptr);
    virtual ~AbstractDocument();

    QSizeF pageSize() const;
    void setPageSize(const QSizeF &newPageSize);

    virtual void print(QPagedPaintDevice *printer) = 0;

private:
    QSizeF pageSize_;
};
}
#endif // ABSTRACTDOCUMENT_H
