import QtQuick
import QtQuick.Controls
import Billing

Item {
    id: root

    readonly property alias count: listView.count
    property alias model: listView.model

    implicitWidth: listView.implicitWidth
    implicitHeight: listView.implicitHeight

    states: [
        State {
            name: "fetching"
            PropertyChanges {
                target: label
                visible: false
            }
            PropertyChanges {
                target: busyIndicator
                running: true
            }
        },
        State {
            name: "hasOrders"
            when: listView.count > 0
            PropertyChanges {
                target: label
                visible: false
            }
        },
        State {
            name: "noOrders"
            PropertyChanges {
                target: busyIndicator
                visible: false
            }
            PropertyChanges {
                target: label
                visible: true
            }
        }
    ]

    ListView {
        id: listView
        anchors.fill: parent
        anchors.topMargin: 5
        delegate: billDelegate
        spacing: 20
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        running: false
    }

    Label {
        id: label
        width: root.width*.8
        anchors.centerIn: parent
        text: "No se ha podido encontrar ninguna orden."
        font.pointSize: 14
        wrapMode: Label.Wrap
    }

    Component {
        id: billDelegate
        CustomerBillDelegate {
            id: customerBillDelegate
            required property var model
            required details
            required payment
            required status
            required customer

            width: root.width
            height: 110
            orderId: model.orderId
            orderDate: model.date
            onClicked: {
                stackView.push("qrc:/qt/qml/Billing/BillDetailsPage.qml", {
                                   details, date: orderDate, payment, orderId, status, customer, model, mode: BillDetailsPage.QueueMode
                               });
            }
        }
    }
}
