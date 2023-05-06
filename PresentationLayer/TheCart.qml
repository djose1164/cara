pragma Singleton
import QtQuick
import com.cara 1.0
import "qrc:/utils"

QtObject {
    id: root

    readonly property ListModel model: ListModel {}

    readonly property var fullOrder: {
        "user_id": TheCurrentUser.userId,
        "customer_id": TheCurrentUser.customerId,
        "date": new Date().toLocaleDateString(Qt.locale(), "dd/MM/yyyy"),
        "order_details": [],
        "paid_amount": 0

    }

    readonly property int totalToPay: {
        const model = TheCart.model;
        const count = model.count;
        const at = idx => model.get(idx);
        let sum = 0;
        for (let i = 0; i < count; ++i)
            sum += at(i).quantity * at(i).price;
        return sum;
    }

    readonly property QtObject _: QtObject {
        function find(func) {
            for (let i = 0; i < model.count; ++i)
                if (func(model.get(i)))
                    return model.get(i);

            return null;
        }

        function productFromModel(productId: int): var {
            return find(item => item.productId === productId);
        }

        function incrementProductQuantityInCart(product: var): void {
            const fetched = productFromModel(product.id);
            ++fetched.quantity;
        }

        // TODO: this should be a method of OrderEntity
        function addProductsToDetails() {
            const model = TheCart.model;
            const count = model.count;
            const details = [];
            for (let i = 0; i< count; ++i)
                details.push({
                    product_id: model.get(i).productId,
                    quantity: model.get(i).quantity,
                    price: model.get(i).price
                });
            fullOrder.order_details = details;
        }
    }

    readonly property CaraApi api: CaraApi {
        id: api

        endpoint: "/orders"
        accessToken: TheCurrentUser.accessToken

        function postOrder(order) {
            post("/", order);
        }
    }

    function addtoCart(product: ProductEntity) {
        model.append(product);
    }

    function removeFromCart(index: int) {
        model.remove(index);
    }

    function isProductInCart(p: var): bool {
        return _.productFromModel(p.productId) !== null;
    }

    function content() {
        const products = [];
        for (let i = 0; i < model.count; ++i)
            products.push(model.get(i));

        return products;
    }

    function checkout() {
        _.addProductsToDetails();
        print(JSON.stringify(fullOrder, null, 2));
        api.postOrder(fullOrder);
    }
}
