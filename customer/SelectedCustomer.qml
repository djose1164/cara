import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import Utils
import "qrc:/customers"

Item {
    id: root

    required property string fullname
    required property CustomerSelector customerSelector
    required property T.StackView stackView

    property alias customerId: customerEdit.customerId

    implicitHeight: 100

    RedirectGroupBox {
        id: groupBox
        anchors.fill: parent
        title: "Cliente seleccionado"
        redirectText: "Editar cliente"
        onRedirectClicked: customerSelector.open()

        CustomerSelectorDelegate {
            anchors.fill: parent
            fullname: root.fullname
            customerId: root.customerId
            onClicked: customerEdit.open()
        }
    }

    CustomerEdit {
        id: customerEdit
        width: app.width
        height: app.height
        stackView: root.stackView
    }
}
