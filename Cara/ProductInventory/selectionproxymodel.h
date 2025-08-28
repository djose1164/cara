#ifndef SELECTIONPROXYMODEL_H
#define SELECTIONPROXYMODEL_H

#include <QItemSelectionModel>
#include <QQmlEngine>
#include <QSortFilterProxyModel>

namespace ProductInventory
{

class SelectionProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QItemSelectionModel *selectionModel READ selectionModel WRITE setSelectionModel
                   NOTIFY selectionModelChanged FINAL)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
    QML_ELEMENT
public:
    explicit SelectionProxyModel(QObject *parent = nullptr) noexcept;

    QItemSelectionModel *selectionModel() const noexcept;
    void setSelectionModel(QItemSelectionModel *newSelectionModel) noexcept;

    int count() const noexcept;

    Q_INVOKABLE QJsonArray toJsonArray() const;

signals:
    void selectionModelChanged();
    void countChanged();

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QPointer<QItemSelectionModel> selectionModel_;
};

} // namespace ProductInventory

#endif // SELECTIONPROXYMODEL_H
