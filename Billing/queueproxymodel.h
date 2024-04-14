#ifndef QUEUEPROXYMODEL_H
#define QUEUEPROXYMODEL_H

#include "abstractsortfiltermodel.h"
#include "orderqueueconstants.h"
#include <QQmlEngine>
#include <QSortFilterProxyModel>

namespace Billing
{
class QueueProxyModel : public Model::AbstractSortFilterModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(OrderQueueEnum::FilterBy status READ status WRITE setStatus NOTIFY statusChanged FINAL)
public:
    explicit QueueProxyModel(QObject *parent = nullptr);

    OrderQueueEnum::FilterBy status() const;
    void setStatus(OrderQueueEnum::FilterBy newStatus);

signals:
    void statusChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    OrderQueueEnum::FilterBy status_;
};
}

#endif // QUEUEPROXYMODEL_H
