import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Shopping
import Cara.User
import Cara.Salesperson
import "qrc:/orders" as Order


Home {
    id: root

    Component.onCompleted: {
        if (UserSettings.userTypeId !== UserEntity.Admin) {
            for (let i = 0; i < model.count; ++i)
                if (model.get(i).name === "Vendedor Asociado")
                    model.remove(i);
        }
    }

    GridView {
        id: grid
        readonly property double prefferedCellSize: {
            if (width <= 500)
                return 180;
            else if (width <= 700)
                return 200;
            else
                return 250;
        }

        anchors.fill: parent
        anchors.margins: 10
        model: model
        delegate: iconOption
        cellWidth: prefferedCellSize
        cellHeight: prefferedCellSize
    }

    Component {
        id: iconOption
        Item {
            id: item
            required property string source
            required property string path
            required property string name
            required property bool showCustomers
            required property bool admin
            required property bool needUser

            width: grid.cellWidth
            height: grid.cellHeight
        ColumnLayout {
            id: column
            anchors.fill: parent
            anchors.margins: 10

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
                    source: item.source
                    sourceSize: Qt.size(50, 50)
                }

                TapHandler {
                    onTapped: {
                        console.log("salespersonId:",SalespersonSettings.salespersonId)
                        if (needUser)
                            homeStack.push(path, {user: UserSettings.entity()});
                        else if (showCustomers)
                            homeStack.push(path, {salespersonId: SalespersonSettings.salespersonId});
                        else if (admin)
                            homeStack.push(path, {adminId: UserSettings.userId});
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
    }

    ListModel {
        id: model

        ListElement {
            source: "qrc:/icons/customers.svg"
            path: "qrc:qt/qml/Cara/Customer/Customers.qml"
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
            path: "qrc:/qt/qml/Cara/Statistics/Statistics.qml"
            name: "Estadisticas"
            showCustomers: true
            admin: false
        }

        ListElement {
            source: "qrc:/qt/qml/Cara/Shopping/catalogue.svg"
            path: "qrc:/qt/qml/Cara/Shopping/Catalogue.qml"
            name: "Catalogo"
        }

        ListElement {
            source: "qrc:/qt/qml/Cara/Salesperson/salesperson.svg"
            path: "qrc:/qt/qml/Cara/Salesperson/AssociateSalesperson.qml"
            name: "Vendedor Asociado"
            needUser: true
        }

        ListElement {
            source: "qrc:/qt/qml/Cara/Billing/orders.svg"
            path: "qrc:/qt/qml/Cara/Billing/OrderQueuePage.qml"
            name: "Cola"
            showCustomers: true
        }
    }
}
