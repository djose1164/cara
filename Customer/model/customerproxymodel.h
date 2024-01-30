#ifndef CUSTOMERPROXYMODEL_H
#define CUSTOMERPROXYMODEL_H

#include <QObject>
#include <QQmlEngine>
#include <QSortFilterProxyModel>

namespace Model
{
class CustomerProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged FINAL)
public:
    explicit CustomerProxyModel(QObject *parent = nullptr);

    QString text() const;
    void setText(const QString &newText);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

signals:
    void textChanged();

private:
    QString text_;
};
}

#endif // CUSTOMERPROXYMODEL_H
