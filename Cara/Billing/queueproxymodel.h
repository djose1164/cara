#ifndef QUEUEPROXYMODEL_H
#define QUEUEPROXYMODEL_H

#include "abstractsortfiltermodel.h"
#include "orderqueue.h"
#include <QQmlEngine>
#include <QSortFilterProxyModel>

namespace Billing
{
class QueueProxyModel : public Model::AbstractSortFilterModel
{
    Q_OBJECT
    Q_PROPERTY(OrderQueue::Status status READ status WRITE setStatus NOTIFY statusChanged FINAL)
    Q_PROPERTY(QString searchString READ searchString WRITE setSearchString NOTIFY searchStringChanged FINAL)
    QML_ELEMENT
public:
    explicit QueueProxyModel(QObject *parent = nullptr);

    OrderQueue::Status status() const;
    void setStatus(OrderQueue::Status newStatus);

    QString searchString() const;
    void setSearchString(const QString &newSearchString);

signals:
    void statusChanged();

    void searchStringChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    OrderQueue::Status status_;
    QString searchString_;
};
}

#endif // QUEUEPROXYMODEL_H
