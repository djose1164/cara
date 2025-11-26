function validateForm(sellerSelector, providerSelector, productSale) {
    if (sellerSelector.sellerId === -1)
        throw "Necesitas seleccionar un vendedor";
    else if (!providerSelector.selected)
        throw "Necesitas seleccionar un suplidor";
    productSale.validate();
}

function buildCommissionItems(productSale, supplierId) {
    const items = productSale.toJson();
    items.forEach(item => item.supplier_id = supplierId);
    return items;
}
