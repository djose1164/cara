import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import "qrc:/orders"

HeaderPage {
    id: root
    readonly property StackView myStack: StackView.view

    title: "Orden de Compra"

    ColumnLayout {
        id: orderBuyForm
        anchors.fill: parent

        ComboBox {
            id: providerComboBox
            model:  ["a", "b"]
        }

        TextField {
            id: date
            placeholderText: "dia/mes/año"
        }

        Label { text: "Poductos" }

        Description {
            id: description
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        TextField {
            id: boughtQuantity
            placeholderText: "Cantidad"
            inputMethodHints: Qt.ImhDigitsOnly
        }

        TextField {
            id: unitPrice
            placeholderText: "Costo de unidad"
        }

        Label {
            id: totalLabel
            text: "Total: " + description.total
        }

        Button {
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignCenter
            text: "Ok"
        }
    }
}
