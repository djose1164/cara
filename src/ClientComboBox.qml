import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import "qrc:/users"
import com.cara 1.0

Item {
    property alias currentText: clientField.currentText
    property alias currentValue: clientField.currentValue

    Component.onCompleted: updateClientField()

    ComboBox {
        id: clientField
        width: parent.width
        editable: true
        currentIndex: 0
        textRole: "name"
        valueRole: "customer_id"
        model: model
    }

    ListModel {
        id: model
    }

    Rest {
        id: rest
        base: "http://127.0.0.1:5000/api/customers/info"
        onSucceeded: resp => model.count ? model.set(
                                               0,
                                               resp.customers) : model.append(
                                               resp.customers)
    }

    function updateClientField() {
        rest.get("/")
    }
}
