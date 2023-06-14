import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import "qrc:/singleton"
import com.cara 1.0

Item {
    property alias currentText: clientField.currentText
    property alias currentValue: clientField.currentValue

    Component.onCompleted: updateClientField()

    ComboBox {
        id: clientField
        anchors.fill: parent
        editable: true
        currentIndex: 0
        textRole: "name"
        valueRole: "customer_id"
        model: model
    }

    ListModel { id: model }

    CustomerAPI {
        id: rest
        onCustomersFetched: customers => (
            model.count ? model.set(
                0,
                customers) : model.append(
                customers
            )
        )
    }

    function updateClientField() {
        rest.get("/")
    }
}
