import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/orders" as Order
import "qrc:/singleton"

Home {
    id: root

    readonly property StackView mainView: ViewManager.mainView

    GridView {
        id: grid
        anchors.fill: parent
        anchors.margins: 20
        model: model
        delegate: iconOption
        cellWidth: width/2
        cellHeight: height*.35
    }

    Component {
        id: iconOption
        ColumnLayout {
            id: column
            required property string source
            required property string path
            required property string name
            width: grid.cellWidth - 15
            height: grid.cellHeight - 10

            Rectangle {
                id: rec
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "chocolate"
                states: [
                    State {
                        name: "hovered"
                        when: control.hovered
                        PropertyChanges {
                            rec {
                                scale: 1.15
                                color: "crimson"
                            }
                        }
                    }
                ]
                transitions: [
                    Transition {
                        PropertyAnimation {
                            property: "scale"
                            duration: 200
                            easing.type: Easing.InOutQuad
                        }

                        ColorAnimation { duration: 200 }
                    }
                ]

                Image {
                    anchors.centerIn: parent
                    source: column.source
                    sourceSize: "50x50"
                }

                TapHandler { onTapped: mainView.push(path) }

                HoverHandler { id: control }
            }

            Label {
                Layout.fillWidth: true
                text: name
                wrapMode: Label.Wrap
            }
        }
    }

    ListModel {
        id: model

        ListElement {
            source: "qrc:/icons/complete_order.svg"
            path: "qrc:/orders/BuyOrder.qml"
            name: "Ordenes de Compra"
        }

        ListElement {
            source: "qrc:/icons/new_product.svg"
            path: "qrc:/product/ProductForm.qml"
            name: "Agregar Producto"
        }

        ListElement {
            source: "qrc:/icons/customers.svg"
            path: "qrc:/customers/Customers.qml"
            name: "Clientes"
        }

        ListElement {
            source: "qrc:/icons/inventory.svg"
            path: "qrc:/inventory/Inventory.qml"
            name: "Inventario"
        }

        ListElement {
            source: "qrc:/icons/supplier.svg"
            path: "qrc:/provider/Providers.qml"
            name: "Proveedores"
        }

        ListElement {
            source: "qrc:/icons/statistics.svg"
            path: "qrc:/statistics/Statistics.qml"
            name: "Estadisticas"
        }
    }
}
