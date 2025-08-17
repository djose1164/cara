#ifndef OUTSTANDINGCUSTOMERSPROXYMODEL_H
#define OUTSTANDINGCUSTOMERSPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QQmlEngine>

namespace Customer
{
class OutstandingCustomersProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString customerName READ customerName WRITE setCustomerName NOTIFY
                   customerNameChanged FINAL)
    QML_ELEMENT
public:
    explicit OutstandingCustomersProxyModel(QObject *parent = nullptr);

    // QSortFilterProxyModel interface
    QString customerName() const;
    void setCustomerName(const QString &newCustomerName);

signals:
    void customerNameChanged();

private:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

    bool ascending_;
    QString customerName_;
};
}

#endif // OUTSTANDINGCUSTOMERSPROXYMODEL_H
