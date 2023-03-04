import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"

Page {
    readonly property StackView myStack: StackView.view

    header: TitleHeader {
        title: "Orden de Compra"
        stackView: myStack
    }

    GridLayout {
        id: orderBuyForm
        width: parent.width
        columns: 2

        ComboBox {
            id: providerComboBox
            model:  ["a", "b"]
        }

        TextField {
            id: date
            placeholderText: "dia/mes/año"
        }

        TextField {
            id: productId
            placeholderText: "Ingrese ID del producto"
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
            text: "Total: " + (boughtQuantity.text && unitPrice.text ?
                                   parseInt(boughtQuantity.text) * parseInt(unitPrice.text) : "0")
        }

        Button {
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignCenter
            text: "Añadir provedor"
        }
    }
}
