import QtQuick
import QtQml
import "qrc:/utils" as Utils

Item {
    id: root
    property alias request: request
    property alias model: model
    readonly property string endpoint: "/products"

    signal succeeded(resp: var)

    Utils.Request {
        id: request
        base: "https://cara-api.onrender.com/api" + endpoint
        onSucceeded: r => root.succeeded(r)
    }

    ListModel { id: model }

    function addProduct(name, desc, buy, sell) {
        const obj = {
            "name": name,
            "description": desc,
            "buy_price": buy,
            "sell_price": sell
        };
        request.post("/", obj, null);
    }

    function findProductByIdentifier(text) {
        print("findProductByIdentifier " + text);
        model.clear();
        request.get("/" + text, null, r => model.append(r.products));
    }
    
    function addSuggetionsToModel() {
        request.get('/', null, resp => {
            model.append(resp.products);
            request.succeeded(resp);
        });
    }
}
