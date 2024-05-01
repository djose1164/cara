#ifndef IPRODUCTCATEGORYREPOSITORY_H
#define IPRODUCTCATEGORYREPOSITORY_H


namespace Catalogue {
class IProductCategoryRepository
{
public:
    virtual ~IProductCategoryRepository();

    virtual void fetchAll() = 0;
};
}

#endif // IPRODUCTCATEGORYREPOSITORY_H
