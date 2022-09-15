import QtQuick

Item {
    id: root
    readonly property int count: clientList.count
    property alias model: clientList

    signal statusReady(bool status)

    ListModel { id: clientList }

    WorkerScript {
        id: worker
        property bool shouldAdd: false
        source: "qrc:/js/client_request.mjs"
        onMessage: response => statusReady(response.code === "success");
    }

    function addById(clientId) {
        worker.sendMessage({"type": "findClientById", "model": clientList, "value": clientId});
    }

    function getAt(index) {
        return clientList.get(index);
    }
}
