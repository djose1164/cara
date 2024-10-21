pragma Singleton
import QtQuick
import Cara.Product

QtObject {
    readonly property ListModel model: ListModel {}

    signal updateModel()

    function modelIsEmpty() {
        return model.count === 0;
    }

    function modelAppend(obj) {
        model.append(obj);
    }

    function modelClear() {
        model.clear();
    }

    function productFromJson(json): product {
        console.log(JSON.stringify(json,null,2))
        return {
            name: json.name,
            categoryId: json.category_id,
            images: json.images,
            price: json.price,
            description: json.description,
            productId: json.id
        };
    }

    function updateProduct(newProduct: product) {
        const product = ProductManager.findById(newProduct.productId);
        product.name = newProduct.name;
        product.description = newProduct.description;
        product.sell_price = newProduct.price;
    }

    function findById(productId: int) {
        console.log("ProductManager.productId:", productId)
        const pred = product => product.id === productId;

        for (let i = 0; i < model.count; ++i)
            if (pred(model.get(i)))
                return model.get(i);

        return null;
    }
}
