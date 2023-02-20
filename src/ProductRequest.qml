import QtQuick
import QtQml
import "qrc:/utils" as Utils

Item {
    property string text
    property alias request: request
    property alias model: model
    readonly property string endpoint: "/products"

    Utils.Request {
        id: request
        base: "http://127.0.0.1:5000/api" + endpoint
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

    function findProductByIdentifier() {
        sender({
            "type": "findProductByName",
            "identifier": text
        });
    }
    
    function addSuggetionsToModel() {
        if (model)
            request.get('/', null, resp => {
                model.append(resp.products);
                request.succeeded(resp);
            });

    }
}
