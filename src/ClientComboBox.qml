import QtQuick
import QtQuick.Controls
import "qrc:/utils"

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

    Request {
        id: request
        base: "https://cara-api.azurewebsites.net/api"
        onSucceeded: resp => clientModel.append(resp.clients)
    }

    Component.onCompleted: updateClientField()

    function updateClientField() {
        request.get("/clients/");
    }
}
