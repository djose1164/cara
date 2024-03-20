import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import Billing
import Utils
import "qrc:/filters"

Page {
    id: root

    readonly property T.StackView stackView: StackView.view ?? billStack
    required property int customerId

    title: "Todas las cuentas"
    header: TitleHeader {
        title: root.title
        btnLoader.sourceComponent: FilterButton {
            onClicked: billFilter.open()
        }

        onBackClicked: stackView.pop()
    }
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

    Component.onCompleted: {
        root.state = "fetching";
        billBusiness.fetchBillsByCustomerId(customerId);
    }

    ListView {
        id: listView
        anchors.fill: parent
        anchors.topMargin: 5
        delegate: billDelegate
        model: billProxyModel
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
                    details, date: orderDate, payment, orderId, status, customer, model
                });
            }
        }
    }

    BillProxyModel {
        id: billProxyModel
        sourceModel: billModel
    }

    BillModel { id: billModel }

    BillBusiness {
        id: billBusiness
        model: billModel
        onOrdersNotFound: root.state = "noOrders"
    }

    BillFilterPopup {
        id: billFilter
        width: root.width * .9
        onAccepted: {
            billProxyModel.orderFilter = orderFilterValue;
            billProxyModel.paymentFilter = paymentFilterValue;
        }
    }
}
