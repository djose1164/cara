#ifndef IEDITABLEPRODUCT_H
#define IEDITABLEPRODUCT_H

namespace EntityLayer { class Product; }
namespace ProductInventory
{

class IEditableProduct
{
public:
    virtual ~IEditableProduct() = default;

    virtual void edit(const EntityLayer::Product &product) = 0;
};
}

#endif // IEDITABLEPRODUCT_H
