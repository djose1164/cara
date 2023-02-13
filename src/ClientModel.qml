import QtQuick
import "qrc:/utils" as Utils

Item {
    id: root

    property alias model: model
    property alias request: request

    signal filtering()

    Utils.Request {
        id: request
        base: "https://cara-api.azurewebsites.net/api/clients"
    }

    ListModel { id: model }

    function setById(clientId) {
        request.get("/" + clientId, null, r => model.set(0, r.client));
    }


    function getClientByName(name) {
        return find(text => text.forename.indexOf(name) !== -1);
    }

    function findAll() {
        request.get("/", null, resp => {
            model.append(resp.clients);
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
}
