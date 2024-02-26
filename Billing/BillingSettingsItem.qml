import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Item {
    id: root

    implicitWidth: row.implicitWidth
    implicitHeight: row.implicitHeight

    Column {
        Row {
            id: row
            Label {
                width: root.width * .65
                text: "Limite maximo de facturas en PDF a almacenar"
                wrapMode: Label.Wrap
            }

            SpinBox {
                id: spinBox
                width: root.width * .28
                height: 30
                from: 1
                to: 256
                value: BillingSettings.maximumSavedPdfFiles
            }
        }

        Button {
            text: "Elimnar todas las facturas"
            onClicked: message.open()
        }
    }

    MessageDialog {
        id: message
        text: "Estamos trabajando en ello"
        onButtonClicked: close()
    }

    Binding {
        target: BillingSettings
        property: "maximumSavedPdfFiles"
        value: spinBox.value
    }
}
