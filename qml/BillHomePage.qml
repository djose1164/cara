import QtQuick
import QtQuick.Controls
import models
import core
import "qrc:/utils"
import "qrc:/filters"
import "qrc:/singleton"

Page {
    id: root

    readonly property StackView stackView: StackView.view
    required property int customerId

    title: "Todas las cuentas"
    header: TitleHeader {
        title: root.title
        btnLoader.sourceComponent: FilterButton {
            onClicked: orderStatusFilter.open()
        }

        onBackClicked: stackView.pop()
    }
    states: [
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

    Component.onCompleted: billBusiness.fetchBillsByCustomerId(customerId)

    OrderStatusFilter {
        id: orderStatusFilter
        width: root.width*.8
        height: root.height/2
        onApplyFilter: statusId => orderStatusPM.statusFilter = statusId
    }

    ListView {
        id: listView
        anchors.fill: parent
        delegate: billDelegate
        model: orderStatusPM

        BusyIndicator {
            id: busyIndicator
            anchors.centerIn: parent
            visible: listView.count === 0
        }

        Label {
            id: label
            width: listView.width*.8
            anchors.centerIn: parent
            visible: false
            text: "Este cliente no cuenta con ninguna orden."
            font.pointSize: 14
            wrapMode: Label.Wrap
        }
    }

    Component {
        id: billDelegate
        CustomerBillDelegate {
            id: customerBillDelegate
            required property var model
             property var details: model.details
             property var payment: model.payment
             property var status: model.status

            width: root.width
            height: 100

            onClicked: {
                stackView.push("qrc:/bills/BillDetailsPage.qml", {
                    details, date, payment, orderId, status
                });
            }

            orderId: model.orderId
            date: model.date
        }
    }

    OrderStatusProxyModel {
        id: orderStatusPM
        sourceModel: billModel
    }

    BillModel { id: billModel }

    BillBusiness {
        id: billBusiness
        model: billModel
        onOrdersNotFound: root.state = "noOrders"
    }
}
