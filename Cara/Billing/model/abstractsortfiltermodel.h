#ifndef ABSTRACTSORTFILTERMODEL_H
#define ABSTRACTSORTFILTERMODEL_H

#include <QObject>
#include <QQmlEngine>
#include <QSortFilterProxyModel>

namespace Model
{

class AbstractSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool filterEnabled READ filterEnabled WRITE setFilterEnabled
                   NOTIFY filterEnabledChanged FINAL)
    QML_ELEMENT
    QML_UNCREATABLE("This is an abstract class to be used only in properties.")

public:
    explicit AbstractSortFilterModel(QObject *parent = nullptr);
    virtual ~AbstractSortFilterModel();

    bool filterEnabled() const;
    void setFilterEnabled(bool newFilterEnabled);

signals:
    void filterEnabledChanged();

private:
    bool filterEnabled_;
};
}

#endif // ABSTRACTSORTFILTERMODEL_H
