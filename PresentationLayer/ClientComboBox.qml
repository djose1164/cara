import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import "qrc:/singleton"
import com.cara 1.0

ComboBox {
    id: root

    Component.onCompleted: if (CustomerManager.modelIsEmpty()) customerApi.fetchCustomers()

    popup.height: 300
    textRole: "name"
    valueRole: "customer_id"
    currentIndex: -1
    displayText: currentIndex === -1 ? "Seleccionar Cliente" : currentText
    model: CustomerManager.model

    CustomerAPI {
        id: customerApi
        onCustomersFetched: customers => CustomerManager.model.append(customers)
    }
}


