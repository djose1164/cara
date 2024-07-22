import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import Cara
import Cara.Utils
import Cara.Billing
import Cara.User
import Cara.Salesperson
import "qrc:/orders"

BackPage {
    id: root

    property AbstractSortFilterModel proxyModel
    required property var model
    readonly property bool taken: root.model.taken
    property int mode

    enum Operation {
        AbandonOrder,
        OrderAbandoned
    }

    enum Mode {
        NormalMode,
        QueueMode,
        QueueOwnerMode // The logged salesperson is the manager of customer.
    }

    title: "Detalles de orden"
    btnLoader.sourceComponent: menuBtn
    Component.onCompleted: if (root.proxyModel) root.proxyModel.filterEnabled = false
    Component.onDestruction: if (root.proxyModel) root.proxyModel.filterEnabled = true

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        OrderState {
            id: orderState
            width: {
                if (root.width <= 400)
                    return root.width * .9;
                else if (root.width <= 500)
                    return root.width * .85
                else if (root.width <= 600)
                    return root.width * .8
                else
                    return root.width *.75
            }
            anchors.horizontalCenter: parent.horizontalCenter
            orderId: root.model.orderId
            details: root.model.details
            date: root.model.date
            status: root.model.status
            model: root.model
            stackView: root.stackView
            paymentDetails.orderId: root.model.orderId
            paymentDetails.salespersonId: SalespersonSettings.salespersonId
            commentsDrawer.height: root.height
        }
    }

    ProcessingIndicator {
        id: processingIndicator
        width: root.width * .8
    }

    Component {
        id: menuBtn
        Row {
            states: [
                State {
                    when: root.mode === BillDetailsPage.QueueOwnerMode
                    PropertyChanges {
                        target: takeAbandonAction
                        enabled: false
                    }
                }
            ]

            RoundButton {
                visible: root.taken && root.mode === BillDetailsPage.QueueMode
                enabled: internal.done
                flat: true
                icon.source: root.model.done ? "check_circle_fill.svg" : "qrc:/icons/done.svg"
                onClicked: if (!root.model.done) spController.markOrderAsDone(root.model.orderId)
            }

            MoreVertButton {
                id: moreVertBtn
                onClicked: menu.open()

                Menu {
                    id: menu
                    y: moreVertBtn.height

                    MenuItem {
                        text: "Exportar como PDF"
                        onTriggered: {
                            processingIndicator.open();
                            orderResource.getOrderById(root.model.orderId);
                        }
                    }

                    MenuItem {
                        id: takeAbandonAction
                        height: visible ? implicitHeight : 0
                        visible: UserSettings.userTypeId !== UserEntity.Customer
                        text: root.taken ? "Abandonar orden" : "Tomar orden"
                        enabled: root.model.payment.status !== Payment.Paid && visible
                        onTriggered: {
                            internal.operation = root.taken ? BillDetailsPage.AbandonOrder : -1;
                            processingIndicator.open();
                            if (root.taken)
                                abandonMessage.open();
                            else
                                spController.takeOrder(root.model.orderId);
                        }
                    }

                    MenuItem {
                        height: visible ? implicitHeight : 0
                        visible: UserSettings.userTypeId !== UserEntity.Customer
                        enabled: !root.model.done && visible
                        text: "Cancelar orden"
                        onTriggered: {

                        }
                    }
                }
            }
        }
    }

    MessageDialog {
        id: abandonMessage
        buttons: MessageDialog.Ok | MessageDialog.Cancel
        text: internal.operation === BillDetailsPage.AbandonOrder
              ? "¿Estas seguro de abandonar esta orden?" : "¡Orden abandonada con exito!"
        onAccepted: {
            // TODO: Abandon this order
            if (internal.operation === BillDetailsPage.AbandonOrder)
                spController.abandonOrder(root.model.orderId)
        }
    }

    MessageDialog {
        id: takeMessage
        text: "¡Orden tomada con exito!"
        buttons: MessageDialog.Ok
    }

    MessageDialog {
        id: errorMessage
        title: "¡Error!"
        buttons: MessageDialog.Ok
    }

    MessageDialog {
        id: doneMessage
        text: "¡Orden marcada como completada con exito!"
    }

    RestService {
        OrderResource {
            id: orderResource
            path: "orders"
            onFetched: resp => {
                orderTemplate.order = JSON.parse(resp).order;
                printer.printBill();
            }
        }
    }

    BillPrinter {
        id: printer
        document: OrderTemplate {
            id: orderTemplate
        }
        onBillPrinted: path => {
            processingIndicator.close();
            ShareUtils.share("file://" + path);
        }
    }

    SalespersonController {
        id: spController
        salespersonId: SalespersonSettings.salespersonId
        onOrderTook: {
            processingIndicator.close();
            root.model.taken = true;
            takeMessage.open();
        }
        onOrderAbandoned: {
            root.model.taken = false;
            internal.operation = BillDetailsPage.OrderAbandoned;
            processingIndicator.close();
            abandonMessage.open();
        }
        onOrderMarkedAsDone: {
            root.model.done = true;
            doneMessage.open();
        }
        onErrorOccurred: {
            processingIndicator.close();
            try {
                errorMessage.text = JSON.parse(data).message;
            } catch (err) {
                errorMessage.text = data;
            } finally {
                errorMessage.open();
            }
        }
    }

    QtObject {
        id: internal
        property int operation: -1
        readonly property bool done: root.model.status.statusId === OrderStatus.Delivered
                                     && root.model.payment.status === Payment.Paid
    }
}
