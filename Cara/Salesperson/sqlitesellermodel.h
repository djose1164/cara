#ifndef SQLITESELLERMODEL_H
#define SQLITESELLERMODEL_H

#include "abstractsellermodel.h"
#include <QSqlQueryModel>
#include <QQmlEngine>

namespace Seller
{

class SQLiteSellerModel : public QSqlQueryModel, public AbstractSellerModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged FINAL)
    QML_ELEMENT
public:
    enum class RoleNames
    {
        FullnameRole = Qt::UserRole + 1,
        ForenameRole,
        SurnameRole,
        SellerIdRole,
    };

    explicit SQLiteSellerModel(QObject *parent = nullptr);

    void refresh();

    // QAbstractItemModel interface
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    // ISellerModel interface
    void insert(int idx, const Entity::Salesperson &seller) override;
    int count() const override;

    // AbstractSellerModel interface
    void setSellers(const QList<Entity::Salesperson> &sellerList) override;

    void setCount(int newCount);

public slots:
    void append(const QVariantMap &seller) override;

signals:
    void countChanged();
    void sellerAdded(int sellerId);

private:
    int count_;
};

} // namespace Seller

#endif // SQLITESELLERMODEL_H
