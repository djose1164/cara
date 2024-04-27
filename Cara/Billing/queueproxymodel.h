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
    Q_PROPERTY(OrderQueueEnum::FilterBy status READ status WRITE setStatus NOTIFY statusChanged FINAL)
    Q_PROPERTY(QString searchString READ searchString WRITE setSearchString NOTIFY searchStringChanged FINAL)
    QML_ELEMENT
public:
    explicit QueueProxyModel(QObject *parent = nullptr);

    OrderQueueEnum::FilterBy status() const;
    void setStatus(OrderQueueEnum::FilterBy newStatus);

    QString searchString() const;
    void setSearchString(const QString &newSearchString);

signals:
    void statusChanged();

    void searchStringChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    OrderQueueEnum::FilterBy status_;
    QString searchString_;
};
}

#endif // QUEUEPROXYMODEL_H
