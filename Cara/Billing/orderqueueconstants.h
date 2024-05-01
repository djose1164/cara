#ifndef ORDERQUEUECONSTANTS_H
#define ORDERQUEUECONSTANTS_H

#include <QObject>
#include <QQmlEngine>

class OrderQueueEnum
{
    Q_GADGET
    QML_VALUE_TYPE(queAs)
public:
    enum class FilterBy
    {
        NoAssignment,
        AssingedToMe,
        Done,
        Any
    };
    Q_ENUM(FilterBy)

    OrderQueueEnum() = default;
};

class OrderQueueEnumDerived : public OrderQueueEnum
{
    Q_GADGET
};

namespace OrderQueueEnumDerivedForeign
{
Q_NAMESPACE
QML_NAMED_ELEMENT(OrderQueueEnum)
QML_FOREIGN_NAMESPACE(OrderQueueEnumDerived)
}

#endif // ORDERQUEUECONSTANTS_H
