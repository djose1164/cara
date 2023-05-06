#include "productentity.h"

using namespace EntityLayer;

ProductEntity::ProductEntity(QObject *parent)
    : QObject{parent}
    , name_{}
    , description_{}
    , price_{}
    , quantity_{}
    , productId_{}
    , imageUrl_{}
{
     QObject::connect(this, &ProductEntity::nameChanged, this, [this]() {
            toMap();
    });
}

QVariantMap ProductEntity::toMap() const
{
    QVariantMap map{};
    const auto metaInfo{metaObject()};

    for (int i{}; i < metaInfo->propertyCount(); ++i)
    {
        const auto property{metaInfo->property(i)};
        const auto name{property.name()};

        if (name != QString{"objectName"})
            map[name] = property.read(this);
    }

    return map;
}
