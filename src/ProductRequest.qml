import QtQuick
import "qrc:/utils" as Utils

Utils.RequestModel {
    property string text

    source: "qrc:/js/product_request.mjs"

    function addProduct(name, desc, buy, sell) {
        sender({"type": "addNewProduct", "value": {
           "name": name,
           "description": desc,
           "buy_price": buy,
           "sell_price": sell
       }});
    }

    function findProductByIdentifier() {
        sender({
            "type": "findProductByName",
            "identifier": text
        });
    }
    
    function addSuggetionsToModel() {
        if (!model)
            return;

        sender({
            "type": "addSuggetionsToModel",
            "identifier": text,
            "model": model
        });
    }
}
