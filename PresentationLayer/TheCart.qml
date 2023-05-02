pragma Singleton
import QtQuick

QtObject {
    id: root

    readonly property ListModel model: ListModel {}

    readonly property QtObject _: QtObject {
        function find(func) {
            for (let i = 0; i < model.count; ++i)
                if (func(model.get(i)))
                    return model.get(i);

            return null;
        }

        function productFromModel(productId: int): var {
            return find(item => item.id === productId);
        }

        function incrementProductQuantityInCart(product: var): void {
            const fetched = productFromModel(product.id);
            ++fetched.quantity;
        }
    }

    function addtoCart(product: var) {
        model.append(product);
    }

    function removeFromCart(index: int) {
        model.remove(index);
    }

    function isProductInCart(product: var): bool {
        return _.productFromModel(product.id) !== null;
    }
}
