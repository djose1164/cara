import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import catalogue
import User
import "qrc:/orders" as Order
import "qrc:/singleton"

Home {
    id: root

    readonly property LocalUserStore userStore: LocalUserStore {}

    Component.onCompleted: {
        if (userStore.user.userType !== UserEntity.Admin) {
            for (let i = 0; i < model.count; ++i)
                if (model.get(i).name === "Vendedor Asociado")
                    model.remove(i);
        }
    }

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
            required property bool showCustomers
            required property bool admin
            required property bool needSalesperson
            required property bool needUser

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

                TapHandler {
                    onTapped: {
                        if (needUser)
                            homeStack.push(path, {user: userStore.user});
                        else if (showCustomers)
                            homeStack.push(path, {salespersonId: userStore.user.salespersonId});
                        else if (admin)
                            homeStack.push(path, {adminId: userStore.user.userId});
                        else if (needSalesperson)
                            homeStack.push(path, {salesperson: userStore.user.salesperson});
                        else
                            homeStack.push(path);
                    }
                }

                HoverHandler { id: control }
            }

            Label {
                Layout.fillWidth: true
                text: name
                wrapMode: Label.Wrap
            }
        }
    }

    LocalUserStore {
        id: userStore
    }

    ListModel {
        id: model

        ListElement {
            source: "qrc:/icons/complete_order.svg"
            path: "qrc:/orders/BuyOrder.qml"
            name: "Ordenes de Compra"
            showCustomers: false
            admin: false
            needSalesperson: true
        }

        ListElement {
            source: "qrc:/icons/customers.svg"
            path: "qrc:/customers/Customers.qml"
            name: "Clientes"
            showCustomers: true
            admin: false
        }

        ListElement {
            source: "qrc:/icons/inventory.svg"
            path: "qrc:/inventory/Inventory.qml"
            name: "Inventario"
            showCustomers: true
            admin: false
        }

        ListElement {
            source: "qrc:/icons/supplier.svg"
            path: "qrc:/provider/Providers.qml"
            name: "Proveedores"
            showCustomers: false
            admin: false
        }

        ListElement {
            source: "qrc:/icons/statistics.svg"
            path: "qrc:/statistics/Statistics.qml"
            name: "Estadisticas"
            showCustomers: true
            admin: false
        }

        ListElement {
            source: "qrc:/qt/qml/catalogue/catalogue.svg"
            path: "qrc:/qt/qml/catalogue/Catalogue.qml"
            name: "Catalogo"
        }

        ListElement {
            source: "qrc:/qt/qml/Salesperson/salesperson.svg"
            path: "qrc:/qt/qml/Salesperson/AssociateSalesperson.qml"
            name: "Vendedor Asociado"
            needUser: true
        }
    }
}
