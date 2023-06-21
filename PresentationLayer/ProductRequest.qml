import QtQuick
import QtQml
import "qrc:/utils"
import "qrc:/singleton"

Item {
    id: root
    property alias request: request

    signal succeeded(products: var)

    CaraApi {
        id: request
        endpoint: "/products"
        accessToken: TheCurrentUser.accessToken
        onSucceeded: resp => root.succeeded(resp.products);
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
        print("findProductByIdentifier " + text);
        request.get("/" + text, null, r => root.succeeded(r.products));
    }
    
    function addSuggetionsToModel() {
        request.get('/');
    }
}
