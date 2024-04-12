#ifndef QUEUEPROXYMODEL_H
#define QUEUEPROXYMODEL_H

#include <QQmlEngine>
#include <QSortFilterProxyModel>
#include "orderqueueconstants.h"

namespace Billing
{
class QueueProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(OrderQueueEnum::FilterBy status READ status WRITE setStatus NOTIFY statusChanged FINAL)
public:
    explicit QueueProxyModel(QObject *parent = nullptr);

    OrderQueueEnum::FilterBy status() const;
    void setStatus(OrderQueueEnum::FilterBy newStatus);

    bool filterEnabled() const;
    void setFilterEnabled(bool newFilterEnabled);

signals:
    void statusChanged();

    void filterEnabledChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    OrderQueueEnum::FilterBy status_;
    bool filterEnabled_;
    Q_PROPERTY(bool filterEnabled READ filterEnabled WRITE setFilterEnabled NOTIFY
                   filterEnabledChanged FINAL)
};
}

#endif // QUEUEPROXYMODEL_H
