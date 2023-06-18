import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import "qrc:/singleton"
import com.cara 1.0

ComboBox {
    id: root

    Component.onCompleted: updateClientField()

    popup.height: 300
    textRole: "name"
    valueRole: "customer_id"
    currentIndex: -1
    displayText: currentIndex === -1 ? "Seleccionar Cliente" : currentText
    model: model

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


