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
}
