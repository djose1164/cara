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
        valueRole: "customer_id"
        model: clientModel
    }

    ListModel { id: clientModel }

    Request {
        id: request
        base: "http://127.0.0.1:5000/api"
        onSucceeded: resp => clientModel.append(resp.customers)
    }

    Component.onCompleted: updateClientField()

    function updateClientField() {
        request.get("/customers/");
    }
}
