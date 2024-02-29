import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Templates as T
import Customer
import User
import Utils
import Inventory
import Product
import entities
import Billing
import Salesperson

Page {
    id: root

    readonly property T.StackView stackView: StackView.view

    header: ToolBar {
        Label {
            anchors.centerIn: parent
            text: "Facturacion"
            font.pointSize: 16
            font.bold: true
        }
    }

    states: [
        State {
            name: "creating"
            PropertyChanges {
                target: busyPopup
                visible: true
            }
            PropertyChanges {
                target: acceptBtn
                enabled: false
            }
        },
        State {
            name: "orderCreated"
            PropertyChanges {
                target: busyPopup
                visible: false
            }
        }
    ]

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth
        contentHeight: columnLayout.implicitHeight

        ColumnLayout {
            id: columnLayout
            width: root.width * .95
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 30

            ChooseCustomer {
                id: chooseCustomer
                Layout.fillWidth: true
            }

            ProductSale {
                id: productSale
                Layout.fillWidth: true
            }

            PaymentInput {
                id: paymentInput
                calculator: productSale.calculator
                Layout.fillWidth: true
            }

            RedirectLabel {
                id: dateRedirectLbl
                title: datePicker.selectedDate.toLocaleDateString()
                font.pointSize: 14
                onClicked: datePicker.open()
            }

            Button {
                id: acceptBtn
                text: "Aceptar"
                Layout.fillWidth: true
                Layout.bottomMargin: 10
                onClicked: acceptHandle()
            }
        }
    }

    DatePicker {
        id: datePicker
        width: root.width * .9
        height: root.height * .75
        onAccepted: print(datePicker.selectedDate.toLocaleDateString())
    }

    MessageDialog {
        id: message
        buttons: MessageDialog.Ok | MessageDialog.Cancel
        onAccepted: {
            if (!internal.canPrint)
                return;
            pdfPrinter.printBill();
            root.state = "creating";
        }
    }

    OrderController {
        id: orderController
        onOrderCreated: newOrder => {
            root.state = "orderCreated";
            message.title = "Exito al crear orden";
            message.informativeText = "¿Deseas exportar la factura en PDF?";
            message.open();
            orderTemplate.order = newOrder.order;
            productSale.refreshSelector();
        }
    }

    BillPrinter {
        id: pdfPrinter
        document: OrderTemplate {
            id: orderTemplate
        }
        onBillPrinted: path => {
            root.state = "";
            shareUtils.share("file://" + path);
        }
    }

    ShareUtils {
        id: shareUtils
    }

    ProcessingIndicator {
        id: busyPopup
        width: root.width * .8
    }

    QtObject {
        id: internal
        readonly property bool canPrint: root.state == "orderCreated"
    }

    function acceptHandle() {
        try {
            validate();
            const orderJson = {
                salesperson: { id: SalespersonSettings.salespersonId },
                customer: chooseCustomer.toJson(),
                products: productSale.toJson(),
                pay: paymentInput.toJson(),
                orderDate: datePicker.toShortDate()
            };
            print(`orderJson: ${JSON.stringify(orderJson, null, 2)}`);
            orderController.createOrder(orderJson);
            root.state = "creating";
        } catch (err) {
            print("NewPurchaseOrder: "+err);
            errorDialog(err);
        }
    }

    function validate() {
        chooseCustomer.validate();
        productSale.validate();
        paymentInput.validate();
    }

    function errorDialog(err: string) {
        message.title = "Error al crear orden"
        message.informativeText = err;
        message.open();
    }
}
