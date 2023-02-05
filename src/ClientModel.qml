import QtQuick
import "qrc:/utils" as Utils

Utils.RequestModel {
    id: root

    signal filtering()

    source: "qrc:/js/client_request.mjs"

    function addById(clientId) {
        send({"type": "findClientById", "value": clientId}, true);
    }

    function getClientByName(name) {
        return find(text => text.forename.indexOf(name) !== -1);
    }

    function findAll() {
        send({"type": "findAll"}, true);
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
