import QtQuick
import QtQml
import Cara.Utils


Item {
    id: root
    property alias request: request

    signal succeeded(products: var)

    CaraApi {
        id: request
        endpoint: "/products"
    }

    function addProduct(name, desc, buy, sell) {
        const obj = {
            "name": name,
            "description": desc,
            "buy_price": buy,
            "sell_price": sell
        };
        request.post("/", obj);
    }

    function findProductByIdentifier(text) {
        print("findProductByIdentifier")
        request.get("/" + text);
        request.succeeded.connect(r => root.succeeded(r.products));
    }
    
    function addSuggetionsToModel(supplierId: int) {
        request.get(`/?supplier_id=${supplierId}`);
        request.succeeded.connect(r => root.succeeded(r.products));
    }
}
