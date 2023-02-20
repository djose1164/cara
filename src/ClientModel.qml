import QtQuick
import "qrc:/utils" as Utils

Item {
    id: root

    property alias model: model
    property alias request: request

    signal filtering()

    Utils.Request {
        id: request
        base: "http://127.0.0.1:5000/api/customers"
    }

    ListModel { id: model }

    function setById(clientId) {
        request.get("/" + clientId, null, r => model.set(0, r.client));
    }

    function addById(clientId) {
        request.get("/" + clientId, null, r => model.append(r.client));
    }

    function getClientByName(name) {
        return find(text => text.forename.indexOf(name) !== -1);
    }

    function findAll() {
        print("Fecthing all customers...");
        request.get("/", null, resp => {
            model.append(resp.customers);
            request.succeeded(resp)
        });
    }

    function add(client) {
        model.append(client);
    }

    function getAt(index) {
        return model.get(index);
    }

    function filter(msg) {
        filtering();
        send(msg);
    }

    function clearModel() {
        model.clear();
    }

    function isModelEmpty() {
        return model.count === 0;
    }
}
