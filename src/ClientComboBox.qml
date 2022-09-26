import QtQuick
import QtQuick.Controls

Item {
    property alias currentText: clientField.currentText
    property alias currentValue: clientField.currentValue

    ComboBox {
        id: clientField
        width: parent.width
        editable: true
        currentIndex: 0
        textRole: "forename"
        valueRole: "id"
        model: clientModel
    }

    ListModel { id: clientModel }

    WorkerScript {
        id: clientWorker
        source: "qrc:/js/client_request.mjs"
    }

    Component.onCompleted: updateClientField()

    function updateClientField() {
        clientWorker.sendMessage({"type": "findAll", "model": clientModel});
    }
}
