import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Drawer {
    width: parent.width
    height: parent.height

    GridLayout {
        anchors.fill: parent
        columns: 2

        Button {
            background: Image {
                source: ":/crossIcon"
            }
        }

        Button {
            text: "Aplicar"
            onClicked: close()
        }

        Label { text: "Ordenar por" }

        ComboBox {
            model: ["Fecha", "Cuentas pagadas", "Cuentas abonadas", "Cuentas por pagar"]
        }
    }
}
