import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import Utils
import "qrc:/customers"

Item {
    id: root

    required property CustomerSelector customerSelector
    required property T.StackView stackView
    required property int customerId

    property alias fullname: customerDelegate.fullname

    implicitHeight: 100

    RedirectGroupBox {
        id: groupBox
        anchors.fill: parent
        title: "Cliente seleccionado"
        redirectText: "Editar cliente"
        onRedirectClicked: customerSelector.open()

        CustomerSelectorDelegate {
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
        stackView: root.stackView
        customerId: root.customerId
    }

    function toJson() {
        return {fullname, id: customerId};
    }
}
