import QtQuick
import QtQml
import "qrc:/utils"
import "qrc:/singleton"

Item {
    id: root
    property alias request: request
    property alias model: model

    signal succeeded()

    CaraApi {
        id: request
        endpoint: "/products"
        accessToken: TheCurrentUser.accessToken
        onSucceeded: resp => {
            model.append(resp.products);
            root.succeeded();
        }
    }

    ListModel { id: model }

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
        model.clear();
        request.get("/" + text, null, r => model.append(r.products));
    }
    
    function addSuggetionsToModel() {
        request.get('/');
    }
}
