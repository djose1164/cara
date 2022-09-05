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
        textRole: "name"
        valueRole: "identifier"
        model: clientModel
    }

    ListModel {
        id: clientModel

        ListElement {
            name: "ID o nombre de cliente"
            identifier: -1
        }
    }

    WorkerScript {
        id: clientWorker
        source: "qrc:/js/client_request.mjs"
    }

    Component.onCompleted: updateClientField()

    function updateClientField() {
        print("Updating client field");
        clientWorker.sendMessage({"type": "findAll", "model": clientModel, "":""});
    }
}
