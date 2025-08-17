import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import Cara.Billing
import Cara.Shopping
import Cara.User
import Cara.Controls
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
        backBtn.visible: stackView.depth > 1
        onBackClicked: stackView.pop()
    }
    states: [
        State {
            name: "fetching"
            PropertyChanges {
                target: busyIndicator
                running: true
            }
        },
        State {
            when: billProxyModel.count === 0
            PropertyChanges {
                label.visible: true
                label.text: "No se ha encontrado ninguna orden con los filtros establecidos."
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
        root.fetchCustomerBills();
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
        visible: false
        text: UserSettings.userTypeId === UserEntity.Customer ? "Aun no has realizado ninguna compra."
                                                              : "No se ha podido encontrar ninguna orden."
        font.pointSize: 14
        wrapMode: Label.Wrap
    }

    Connections {
        target: CartController

        function onCheckoutSucceeded() {
            billModel.clear();
            root.fetchCustomerBills();
        }
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
                stackView.push("qrc:/qt/qml/Cara/Billing/BillDetailsPage.qml", {
                                   model, proxyModel: billProxyModel,
                                   mode: BillDetailsPage.QueueOwnerMode
                               });
            }
        }
    }

    BillProxyModel {
        id: billProxyModel
        sourceModel: billModel
        onCountChanged: console.log("BillProxyModel.count: "+count)
    }

    BillModel { id: billModel }

    BillBusiness {
        id: billBusiness
        model: billModel
        onOrdersNotFound: root.state = "noOrders"
        onPopulated: root.state = "hasOrders"
    }

    BillFilterPopup {
        id: billFilter
        width: root.width * .9
        onAccepted: {
            billProxyModel.orderFilter = orderFilterValue;
            billProxyModel.paymentFilter = paymentFilterValue;
        }
    }

    function fetchCustomerBills() {
        root.state = "fetching";
        billBusiness.fetchBillsByCustomerId(customerId);
    }
}
