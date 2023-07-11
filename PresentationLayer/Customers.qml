import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import "qrc:/singleton"

HeaderPage {
    id: root

    readonly property int fontSize: 12

    Component.onCompleted: if (CustomerManager.modelIsEmpty()) customerApi.fetchCustomers()

    title: "Clientes"

    ListView {
        id: listView
        anchors.fill: parent
        model: CustomerManager.model
        delegate: SwipeDelegate {
            id: control

            required property int customer_id

            required property string name

            onClicked: {
                customerEdit.customerId = customer_id;
                customerEdit.open();
            }

            width: listView.width
            text: `${name}\nClient ID: ${customer_id}`
            contentItem: Label {
                width: control.width
                text: control.text
                font.pointSize: fontSize
            }

            swipe.right: Row {
                component SwipeLabel: Label {
                    property alias backgroundColor: background.color

                    height: parent.height
                    font.pointSize: fontSize
                    verticalAlignment: Label.AlignVCenter
                    padding: 12
                    color: "white"
                    background: Rectangle {
                        id: background
                        color: "blue"
                    }
                }

                height: parent.height
                anchors.right: parent.right

                SwipeLabel {
                    text: "Eliminar"
                    backgroundColor: 'tomato'
                }
            }
        }

        Label {
            width: listView.width * .9
            anchors.centerIn: parent
            visible: listView.count === 0
            text: "La lista de clientes esta vacia. Inteta agregar uno nuevo."
            font.pointSize: 14
            wrapMode: Label.Wrap
        }
    }

    RoundButton {
        onClicked: stackView.push("qrc:/customers/CustomerForm.qml")

        anchors {
            right: parent.right
            rightMargin: 10
            bottom: parent.bottom
            bottomMargin: 15
        }

        icon.source: "qrc:/icons/person_add.svg"
        icon.width: 30
        icon.height: 30
        Material.background: Material.Blue
        Material.foreground: "white"
    }

    CustomerAPI {
        id: customerApi
        onCustomersFetched: customers => listView.model.append(customers);
    }

    CustomerEdit {
        id: customerEdit
        width: root.width
        height: root.height
    }
}
