import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import Utils
import Customer

Item {
    id: root

    property int customerId

    property alias fullname: customerDelegate.fullname

    signal changeCustomerClicked

    implicitHeight: 125

    RedirectGroupBox {
        id: groupBox
        anchors.fill: parent
        title: "Cliente seleccionado"
        redirectText: "Editar cliente"
        onRedirectClicked: root.changeCustomerClicked()

        CustomerCard {
            id: customerDelegate
            anchors.fill: parent
            customerId: root.customerId
            onClicked: customerEdit.open()
        }
    }

    CustomerEdit {
        id: customerEdit
        width: app.width
        height: app.height
        customerId: root.customerId
    }

    function toJson() {
        return {fullname, id: customerId};
    }
}
