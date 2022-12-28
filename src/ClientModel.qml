import QtQuick
import "qrc:/utils" as Utils

Utils.RequestModel {
    id: root

    signal statusReady(bool status)
    scriptSource: "qrc:/js/client_request.mjs"
    onCodeChanged: code => print("[ClientModel] code:" + code)

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
        print("## model.count:" + model.count);
        print(JSON.stringify(model.get(0)));
    }

    function getAt(index) {
        return model.get(index);
    }

    function clearModel() {
        model.clear();
    }
}
