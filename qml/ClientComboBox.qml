import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import "qrc:/singleton"
import core 1.0

ComboBox {
    id: root

    popup.height: 300
    textRole: "name"
    valueRole: "customer_id"
    currentIndex: -1
    displayText: currentIndex === -1 ? "Seleccionar Cliente" : currentText
    model: CustomerManager.model

    CustomerAPI {
        id: customerApi
        onCustomersFetched: customers => CustomerManager.addCustomer(customers)
    }
}
