import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import Utils
import Billing
import User
import Salesperson
import "qrc:/orders"

BackPage {
    id: root

    required property var model
    property int orderId
    property var date
    property var details
    property payment payment
    property var status
    readonly property bool taken: model.taken
    property alias customer: orderState.buyer

    enum Operation {
        AbandonOrder,
        OrderAbandoned
    }

    title: "Detalles de orden"
    btnLoader.sourceComponent: menuBtn

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
            orderId: root.orderId
            details: root.details
            date: root.date
            status: root.status
            model: root.model
        }
    }

    ProcessingIndicator {
        id: processingIndicator
        width: root.width * .8
    }

    Component {
        id: menuBtn
        Row {
            RoundButton {
                visible: root.taken
                enabled: internal.done
                flat: true
                icon.source: model.done ? "check_circle_fill.svg" : "qrc:/icons/done.svg"
                onClicked: if (!model.done) spController.markOrderAsDone(root.orderId)
            }

            MoreVertButton {
                onClicked: menu.open()

                Menu {
                    id: menu

                    MenuItem {
                        text: "Exportar como PDF"
                        onTriggered: {
                            processingIndicator.open();
                            orderResource.getOrderById(root.orderId);
                        }
                    }

                    MenuItem {
                        visible: UserSettings.userTypeId !== UserEntity.Customer
                        text: root.taken ? "Abandonar orden" : "Tomar orden"
                        enabled: root.payment.status !== Payment.Paid
                        onTriggered: {
                            internal.operation = root.taken ? BillDetailsPage.AbandonOrder : -1;
                            processingIndicator.open();
                            if (root.taken)
                                abandonMessage.open();
                            else
                                spController.takeOrder(root.orderId);
                        }
                    }

                    MenuItem {
                        visible: UserSettings.userTypeId !== UserEntity.Customer
                        enabled: !model.done
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
                spController.abandonOrder(root.orderId)
        }
    }

    MessageDialog {
        id: takeMessage
        text: "¡Orden tomada con exito!"
        buttons: MessageDialog.Ok
    }

    MessageDialog {
        id: errorMessage
        text: "¡Error!"
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
            model.taken = true;
            takeMessage.open();
        }
        onOrderAbandoned: {
            model.taken = false;
            internal.operation = BillDetailsPage.OrderAbandoned;
            processingIndicator.close();
            abandonMessage.open();
        }
        onOrderMarkedAsDone: {
            model.done = true;
            doneMessage.open();
        }
    }

    QtObject {
        id: internal
        property int operation: -1
        readonly property bool done: root.status.statusId === OrderStatus.Delivered
                                     && root.payment.status === Payment.Paid
    }
}
