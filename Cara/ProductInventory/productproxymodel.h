#ifndef PRODUCTPROXYMODEL_H
#define PRODUCTPROXYMODEL_H

#include <QObject>
#include <QQmlEngine>
#include <QSortFilterProxyModel>

namespace ProductInventory
{
class ProductProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString productName READ productName WRITE setProductName NOTIFY productNameChanged FINAL)
public:
    explicit ProductProxyModel(QObject *parent = nullptr);

    QString productName() const;
    void setProductName(const QString &newProductName);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

signals:
    void productNameChanged();

private:
    QString productName_;
};
}

#endif // PRODUCTPROXYMODEL_H
