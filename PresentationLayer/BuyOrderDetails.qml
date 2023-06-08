import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import "qrc:/product"

HeaderPage {
    id: root

    readonly property StackView stackView: StackView.view
    property int orderId
    property var payment
    property var orderDetails
    property var provider
    property var contact: provider.contact
    property string description
    property string date

    title: "Detalles de Orden"

    GridLayout {
        anchors.fill: parent
        anchors.margins: 5
        columns: 2

        GroupBox {
            Layout.rowSpan: 2
            Layout.alignment: Qt.AlignTop
            Layout.fillHeight: true
            title: "Proveedor"

            ColumnLayout {
                anchors.fill: parent

                Label {
                    text: `Nombre: ${provider.name}`
                }

                Label {
                    text: `ID: ${provider.id}`
                }

                Label {
                    text: `Email: ${contact.email}`
                }

                Label {
                    text: `Telefono: ${contact.telephone}`
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true
            title: "Descripcion"
            Label {
                width: parent.width
                text: `${description}`
                wrapMode: Label.Wrap
            }
        }

        GroupBox {
            title: "Fecha"
            Label {
                text: `${date}`
            }
        }

        GroupBox {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            Layout.fillHeight: true

            title: "Detalles"

            ListView {
                id: listView
                anchors.fill: parent
                spacing: 20
                model: orderDetails
                delegate: ProductCard {
                    required property var product
                    required property int quantity

                    width: listView.width
                    height: 100
                    imageUrl: product.image_url
                    productName: product.name
                    productPrice: product.buy_price
                    spinBox.value: quantity
                    total: product.buy_price * quantity
                }
            }
        }
    }
}
