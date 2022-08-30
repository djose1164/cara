import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    header: ToolBar {
        RowLayout {
            anchors.fill: parent

            Image {
                source: "qrc:/icons/left_arrow.svg"
                sourceSize: iconSize
                antialiasing: false
            }

            Label {
                Layout.alignment: Qt.AlignCenter
                text: "Orden de compra"
                horizontalAlignment: Label.horizontalCenter
            }
        }
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
   }
}
