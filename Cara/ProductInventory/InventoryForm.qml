import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara.Controls
import Cara.Product

Page {
    id: root

    readonly property T.StackView stackView: StackView.view
    required property var product
    required property var warehouse
    required property int inventoryId
    required property int minimunStockLevel
    required property int maximunStockLevel
    required property int reorderPoint
    required property int quantityAvailable

    title: "Stocks"
    header: ToolBar {
        RowLayout {
            anchors.fill: parent

            BackButton {
                onClicked: stackView.pop()
            }

            Label {
                text: root.title
                font.pointSize: 14
                font.bold: true
                Layout.alignment: Qt.AlignLeft
            }
        }
    }

    component SpinBoxLabeled : Item {
        property alias text: label.text
        property alias value: spinBox.value

        width: root.width/2
        height: 100

        Label {
            id: label
        }

        SpinBox {
            id: spinBox
            anchors.top: label.bottom
            anchors.topMargin: 20
            enabled: false
        }
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        Column {
            id: column
            width: root.width*.8
           // height: root.height*1.1
            anchors.centerIn: parent
            spacing: 25

            Image {
                source: product.image_url
                sourceSize: "200x200"
            }

            SpinBoxLabeled {
                text: "En stock"
                value: quantityAvailable
            }

            Label {
                text: "Nivel minimo de stokcs"
            }

            SpinBox {
                value: minimunStockLevel
            }

            Label {
                text: "Nivel maximo de stokcs"
            }

            SpinBox {
                value: maximunStockLevel
            }

            Label {
                text: "Punto de abastecimiento"
            }

            SpinBox {
                value: reorderPoint
            }

            Label {
                text: "Almacenado en"
            }

            TextField {
                width: column.width
                placeholderText: "Almacen"
                text: warehouse.name
            }

            Row {
                Button {
                    text: "Cancelar"
                }

                Button {
                    text: "Guardar"
                    Material.background: Material.Green
                }
            }
        }
    }
}
