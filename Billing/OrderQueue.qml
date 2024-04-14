import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Utils
import Billing

BackPage {
    id: root
    title: "Cola"
    btnLoader.sourceComponent: menuBtn

    states: [
        State {
            when: internal.busy
            PropertyChanges {
                target: busyIndicator
                running: true
            }
        },
        State {
            when: !internal.busy && listView.count === 0
            PropertyChanges {
                target: messageLbl
                text: "No hay ninguna orden para mostrar."
            }
        }
    ]

    Component.onCompleted: {
        fetchOrdersInQueue();
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        running: false
    }

    Label {
        id: messageLbl
        width: root.width * .8
        visible: false
        wrapMode: Label.Wrap
        horizontalAlignment: Label.AlignHCenter
        font.pointSize: 16
    }

    Component {
        id: menuBtn
        MoreVertButton {
            onClicked: menu.open()

            Menu {
                id: menu

                MenuItem {
                    text: "Filtro"
                    onTriggered: {
                        // TODO: Open filter dialog.
                        filterDialog.open();
                    }
                }
            }
        }
    }

    Dialog {
        id: filterDialog
        width: root.width * .8
        height: Math.max(root.height/2, implicitHeight)
        anchors.centerIn: Overlay.overlay
        title: "Filtro"
        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            print("Queue: "+internal.queueStatus);
            queueProxy.status = internal.queueStatus;
        }

        ButtonGroup {
            buttons: column.children
        }

        Column {
            id: column
            RadioButton {
                text: "Sin asignar"
                onClicked: internal.queueStatus = OrderQueueEnum.NoAssignment
            }

            RadioButton {
                text: "Asignadas a mi"
                onClicked: internal.queueStatus = OrderQueueEnum.AssingedToMe
            }

            RadioButton {
                text: "Completadas"
                onClicked: internal.queueStatus = OrderQueueEnum.Done
            }

            RadioButton {
                text: "Todas"
                checked: true
                onClicked: internal.queueStatus = OrderQueueEnum.Any
            }
        }
    }

    QueueProxyModel {
        id: queueProxy
        sourceModel: billModel
    }

    ListView {
        id: listView
        anchors.fill: parent
        anchors.topMargin: 5
        delegate: billDelegate
        spacing: 20
        model: queueProxy
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
                stackView.pushItem("qrc:/qt/qml/Billing/BillDetailsPage.qml", {
                                   model, mode: BillDetailsPage.QueueMode, proxyModel: queueProxy
                               });
            }
        }
    }

    BillBusiness {
        id: billBusiness
        model: billModel
        onOrdersNotFound: internal.busy = false
        onPopulated: internal.busy = false
    }

    BillModel { id: billModel }

    QtObject {
        id: internal
        property int queueStatus
        property bool busy
    }

    function setFetchingState() {

    }

    function fetchOrdersInQueue() {
        internal.busy = true;
        setFetchingState();
        billBusiness.fetchOrdersInQueue();
    }

    function fetchCustomerBills(customerId: int) {
        setFetchingState();
        billBusiness.fetchBillsByCustomerId(customerId);
    }
}
